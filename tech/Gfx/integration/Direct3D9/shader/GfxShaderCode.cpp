/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#include "stdafx.h"
#include "GfxShaderCode.h"

using namespace CoS;
//---------------------------------------------------------------------------
/*
	Shader constant assignments:

	Vertex:
	-----------------------------------------------------------------------
			** these are constant per frame **
	0..3	View matrix
	4..7	ViewProj matrix
	8..11	Light 0..3 world position (attenuation in .w component)

			** these (can, and usually will) change per render **
	12..15	World matrix
	16..19	WorldViewProjection matrix
	20..23	WorldViewInverseTranspose matrix
	24..31	??? UVW matrices perhaps ???
	32..255	Matrix Palette (72 bones) -- steal from top of these if needed
*/
//---------------------------------------------------------------------------
const char* GfxShaderCode::vs[MAX_VS_PROFILES][MAX_VS_SNIPPETS] =
{
	// VS_2_0
	{
	},

	// VS_3_0
	{
		// GLOBALS
		"float4x4 ViewProj : VIEWPROJ; \n"
		"float4 LightPos[4] : LIGHTPOS; \n"
		"float4x4 World : WORLD; \n"
		"float4x4 WorldViewInv : WORLDVIEWINV; \n"
		"float4x4 WVP : WORLDVIEWPROJ; \n"
		"float4x4 ViewInv : VIEWINV; \n"
		"float4x4 TexMat1 : TEXMAT1; \n"
		"float4x4 TexMat2 : TEXMAT2; \n"
		"float4 LightDir[4] : LIGHTDIR; \n"
		"float4x4 View : VIEW; \n"
		"float4 Bones[208] : MATRIXPALETTE; \n"
		"\n"

		// VS_IN open
		,"struct VS_IN \n"
		"{ \n"
		"	float4 pos : POSITION; \n"

		// VS_IN close
		,"}; \n"
		"\n"

		// VS_MAIN open 
		,"PS_IN VS_MAIN(VS_IN vsIN) \n"
		"{ \n"
		"	PS_IN psIN; \n"
		//"	psIN.hPos = doWVP(WVP, vsIN.pos); \n"
		"	\n"

		// VS_MAIN close
		,"	return psIN; \n"
		"}\n"
		"\n"

		// WVP transform
		,"float4 doWVP(float4x4 wvp, float4 pos) \n"
		"{ \n"
		"	return mul(pos, wvp); \n"
		"} \n"
		"\n"

		// position skinning
		,"float4 doSkin(float4 pos, const float bone, const float wt)\n"
		"{\n"
		"	float4 rtn;\n"
		"	int c = bone * 3;\n"
		"	rtn.x = dot(Bones[c], pos);\n"
		"	rtn.y = dot(Bones[c+1], pos);\n"
		"	rtn.z = dot(Bones[c+2], pos);\n"
		"	rtn.w = pos.w;\n"
		"	return rtn * wt;\n"
		"}\n"
		"\n"
		"float4 doSkinning(float4 pos, float4 ind, float4 wt)\n" 
		"{ \n"
		"	float4 rtn;\n"
		"	rtn  = doSkin(pos, ind.x, wt.x);\n"
		"	rtn += doSkin(pos, ind.y, wt.y);\n"
		"	rtn += doSkin(pos, ind.z, wt.z);\n"
		"	rtn += doSkin(pos, ind.w, wt.w);\n"
		"	return rtn;\n"
		"} \n"
		"\n"

		// wvp the position
		,"	psIN.hPos = doWVP(WVP, vsIN.pos);\n"

		// skin the position
		,"	float oo255 = 1.0f / 255.0f;\n"
		"	psIN.hPos = doSkinning(vsIN.pos, vsIN.blendIdx, vsIN.blendWeight*oo255);\n"
//		"	psIN.hPos = mul(WVP, psIN.hPos);\n"
		"	psIN.hPos = mul(psIN.hPos, ViewProj);\n"
		"\n"

		// normal transform
		,"float4 doNormal(float3x3 m, float4 n) \n"
		"{ \n"
		"	float4 rtn;\n"
		"	rtn.xyz = normalize(mul(n.xyz, m));\n"
		"	rtn.w = n.w;\n"
		"	return rtn;\n"
		"} \n"
		"\n"

		// normal transform (skinned)
		,"float4 doNormalSkinned(float4 n, float4 indices, float4 weights) \n"
		"{ \n"
		//"	float4 rtn;\n"
		//"	rtn.xyz = normalize(mul(n.xyz, m));\n"
		//"	rtn.w = n.w;\n"
		//"	return rtn;\n"
		"	return n;\n"
		"} \n"
		"\n"

		// wvp the normal
		,"	psIN.norm = doNormal((float3x3)WorldViewInv, vsIN.norm);\n"

		// skin the normal
		,"	psIN.norm = doSkinning(vsIN.norm, vsIN.blendIdx, vsIN.blendWeight*oo255);\n"
		"	psIN.norm = mul(psIN.norm, ViewProj);\n"
		"	psIN.norm = mul(psIN.norm, WorldViewInv);\n"

		// texCoord transform
		,"float4 doTexCoord(float4x4 m, float4 t) \n"
		"{ \n"
		"	return mul(t, m));\n"
		"} \n"
		"\n"

		// VS color copy snippet
		,"	psIN.col0 = vsIN.col0;\n"
		"\n"

		// light vector calculation -- spots and directional get passed through, 
		// points have to have their calculated from the obj and light positions.
		// The engine fills the .w with whether pos and dir are valid, based on 
		// light type. We in turn use the .w in the light vec to let the PS know
		// whether the light is a spot or not (if it has both position and 
		// direction, then it's a spot).
		,""
		"float4 getLightVec(float4 world, float4 lightPos, float4 lightDir)\n"
		"{\n"
		"	float4 rtn = lightDir;\n"
		"	rtn.w = lightDir.w * lightPos.w;\n"
		"\n"
		"	if (lightDir.w < 0.5)\n"
		"	{\n"
		"		rtn.xyz = (lightPos.xyz - world.xyz);\n"
		"	}\n"
		"\n"
		"	return rtn;\n"
		"}\n"
		"\n"

		// custom shaders -- source file name (no ext), /, and entry point
		,"TerrainShader"	// SHADER_TERRAIN
		,"MACROShader"		// SHADER_MACRO
		,"PropShader"		// SHADER_PROP
		,"ParticleShader"	// SHADER_VFX
	}
};
//---------------------------------------------------------------------------
/*
	Shader constant assignments:
	
	Pixel:
	-----------------------------------------------------------------------
			** these are constant per frame **
	0..3	reserved for compiler constants
	4..7	Light 0..3 color
	8..11	Light 0..3 specular color
	12		Ambient

			** these (can, and usually will) change per render **
	28..31	Material colors (diff, amb, spec, emissive)
*/
//---------------------------------------------------------------------------
const char* GfxShaderCode::ps[MAX_PS_PROFILES][MAX_PS_SNIPPETS] = 
{
	// PS_2_0
	{
	},

	// PS_3_0
	{
		// GLOBALS
		"float4 LightCol[4]		: LIGHTCOL;\n"
		"float4 LightSCol[4]	: LIGHTSCOL;\n"
		"float4 Ambient			: WORLDAMBIENT;\n"
		"float4 DiffuseCol		: DIFFUSECOL;\n"
		"float4 AmbientCol		: AMBIENTCOL;\n"
		"float4 SpecularCol		: SPECULARCOL;\n"
		"float4 EmissiveCol		: EMISSIVECOL;\n"
		"\n"


		// PS_IN open
		,"struct PS_IN \n"
		"{ \n"
		"	float4 hPos : POSITION; \n"

		// PS_IN close
		,"}; \n"
		"\n"

		// PS_MAIN open 
		,"float4 PS_MAIN(PS_IN psIN) : COLOR0\n"
		"{ \n"
		"	float4 oCol = saturate(DiffuseCol + Ambient * AmbientCol); \n"

		// PS_MAIN close
		,"	return oCol; \n"
		"} \n"
		"\n"

		// 2D sampler code
		,"float4 sample2D(sampler2D samp, float4 tex)\n"
		"{\n"
		"	return tex2D(samp, tex.xy);\n"
		"}\n"
		"\n"

		// 3D sampler code
		,"float4 sample3D(sampler3D samp, float4 tex)\n"
		"{\n"
		"	return tex3D(samp, tex.xyz);\n"
		"}\n"
		"\n"

		// CUBE sampler code
		,"float4 sampleCUBE(samplerCUBE samp, float4 tex)\n"
		"{\n"
		"	return texCUBE(samp, tex.xyz);\n"
		"}\n"
		"\n"

		// Blinn shading
		,"float Eccentricity = 0.4;\n"
		"float Ks = 0.1;\n"
		"void doBlinn(\n"
		"		float3 lightColor,\n"
		"		float3 Nn,\n"
		"		float3 Ln,\n"
		"		float3 Vn,\n"
		"	out	float3 diffContrib,\n"
		"	out	float3 specContrib\n"
		"	)\n"
		"{\n"
		"	float3 Hn = normalize(Vn + Ln);\n"
		"	float hdn = dot(Hn,Nn);\n"
		"	float3 R = reflect(-Ln,Nn);\n"
		"	float rdv = dot(R,Vn);\n"
		"	rdv = max(rdv,0.001);\n"
		"	float ldn=dot(Ln,Nn);\n"
		"	ldn = max(ldn,0.0);\n"
		"	float ndv = dot(Nn,Vn);\n"
		"	float hdv = dot(Hn,Vn);\n"
		"	float eSq = Eccentricity*Eccentricity;\n"
		"	float distrib = eSq / (rdv * rdv * (eSq - 1.0) + 1.0);\n"
		"	distrib = distrib * distrib;\n"
		"	float Gb = 2.0 * hdn * ndv / hdv;\n"
		"	float Gc = 2.0 * hdn * ldn / hdv;\n"
		"	float Ga = min(1.0,min(Gb,Gc));\n"
		"	float fresnelHack = 1.0 - pow(ndv,5.0);\n"
		"	hdn = distrib * Ga * fresnelHack / ndv;\n"
		"	diffContrib = ldn * lightColor;\n"
		"	specContrib = hdn * Ks * lightColor;\n"
		"}\n"
		"\n"

		// invoke the blinn lighting calculations
		,"\t// calculate Blinn shading\n"
		"	float3 diffContrib={0,0,0};\n"
		"	float3 specContrib={0,0,0};\n"
		"	float3 Vn = normalize(psIN.worldView);\n"
		"	float3 Nn = normalize(psIN.norm);\n"
		"	float3 d, s;\n"
		"\n"
		"	float3 Ln = normalize(psIN.light0vec);\n"
		"	doBlinn(LightCol[0], Nn, Ln, Vn, d, s);\n"
		"	diffContrib += d; specContrib += s;\n"
		"	//Ln = normalize(psIN.light1vec);\n"
		"	//doBlinn(LightCol[1], Nn, Ln, Vn, d, s);\n"
		"	//diffContrib += d; specContrib += s;\n"
		"	//Ln = normalize(psIN.light2vec);\n"
		"	//doBlinn(LightCol[2], Nn, Ln, Vn, d, s);\n"
		"	//diffContrib += d; specContrib += s;\n"
		"	//Ln = normalize(psIN.light3vec);\n"
		"	//doBlinn(LightCol[3], Nn, Ln, Vn, d, s);\n"
		"	//diffContrib += d; specContrib += s;\n"
		"\n"
		"	oCol.rgb = min(1, oCol * (diffContrib + Ambient));\n"
		"\n"

		// custom shaders -- source file name (no ext), /, and entry point
		,"TerrainShader/PS_MAIN"	// SHADER_TERRAIN
		,"MACROShader/PS_MAIN"		// SHADER_MACRO
		,"PropShader/PS_MAIN"		// SHADER_PROP
		,"ParticleShader/PS_MAIN"	// SHADER_VFX
	}
};

