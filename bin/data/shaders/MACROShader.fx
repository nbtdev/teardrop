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

float4x4 WorldITXf : WORLDINVTRANS;
float4x4 WvpXf : WORLDVIEWPROJ;
float4x4 WorldXf : WORLD;
float4x4 WorldInv : WORLDINV;
float4x4 ViewIXf : VIEWINV;
float4x4 ViewProj : VIEWPROJ;

float3 Lamp0Pos[1] : LIGHTPOS;
float3 Lamp0Color[1] : LIGHTCOL;
float3 AmbiColor : WORLDAMBIENT;
float Ks = 0.2;
float Eccentricity = 0.05;
float4 Bones[208] : MATRIXPALETTE; 
sampler2D ColorSampler;

struct appdata {
    float4 Position	: POSITION;
    float4 UV		: TEXCOORD0;
    float4 Normal	: NORMAL;
    //float4 Tangent	: TANGENT0;
    //float4 Binormal	: BINORMAL0;
	float4 BlendWeight : BLENDWEIGHT;
	float4 BlendIdx : BLENDINDICES;
};

struct vertexOutput {
    float4 HPosition	: POSITION;
    float2 UV		: TEXCOORD0;
    // The following values are passed in "World" coordinates since
    //   it tends to be the most flexible and easy for handling
    //   reflections, sky lighting, and other "global" effects.
    float3 LightVec	: TEXCOORD1;
    float3 WorldNormal	: TEXCOORD2;
    float3 WorldTangent	: TEXCOORD3;
    float3 WorldBinormal : TEXCOORD4;
    float3 WorldView	: TEXCOORD5;
};

float4 doSkin(float4 pos, const float bone, const float wt)
{
	float4 rtn;
	int c = bone * 3;
	rtn.x = dot(Bones[c], pos);
	rtn.y = dot(Bones[c+1], pos);
	rtn.z = dot(Bones[c+2], pos);
	rtn.w = pos.w;
	return rtn * wt;
}

float4 doSkinning(float4 pos, float4 ind, float4 wt)
{ 
	float4 rtn;
	rtn  = doSkin(pos, ind.x, wt.x);
	rtn += doSkin(pos, ind.y, wt.y);
	rtn += doSkin(pos, ind.z, wt.z);
	rtn += doSkin(pos, ind.w, wt.w);
	return rtn;
} 

/****************************************************************
	Standard Rendering
****************************************************************/

vertexOutput std_VS(appdata IN) {
    vertexOutput OUT = (vertexOutput)0;

    OUT.UV = float2(IN.UV.x,(1.0-IN.UV.y));

	float oo255 = 1.0f / 255.0f;
	// todo: support more than 1 bone
	OUT.HPosition = doSkinning(IN.Position, IN.BlendIdx, IN.BlendWeight*oo255);

    float3 Pw = OUT.HPosition.xyz;
    OUT.LightVec = (Lamp0Pos[0] - Pw);
    OUT.WorldView = normalize(ViewIXf[3].xyz - Pw);
    OUT.HPosition = mul(OUT.HPosition,ViewProj);

	// since the world matrix is in the bones array, and we normally would want to multiply the 
	// normal by the WorldInvTrans, we need to change the order of the mul() operands (which effects
	// the transpose on the bone matrices) and multiply against the WorldInv (which will give us the WorldInvTrans
	// we're looking for)
	// todo: support more than one bone
	float4 worldNormal = doSkinning(IN.Normal, IN.BlendIdx, IN.BlendWeight*oo255);
    OUT.WorldNormal = mul(WorldInv, worldNormal).xyz;
	normalize(OUT.WorldNormal);

	// need to do the same for the tangent/binormal
	//float4 worldTangent = doSkinning(IN.Tangent, IN.BlendIdx, IN.BlendWeight*oo255);
    //OUT.WorldTangent = mul(WorldInv, worldTangent).xyz;
	//normalize(OUT.WorldTangent);

    //OUT.WorldBinormal = cross(OUT.WorldTangent, OUT.WorldNormal);

    return OUT;
}

// Utility function for blinn shading

void blinn_shading(vertexOutput IN,
		    float3 LightColor,
		    float3 Nn,
		    float3 Ln,
		    float3 Vn,
		    out float3 DiffuseContrib,
		    out float3 SpecularContrib)
{
    float3 Hn = normalize(Vn + Ln);
    float hdn = dot(Hn,Nn);
    float3 R = reflect(-Ln,Nn);
    float rdv = dot(R,Vn);
    rdv = max(rdv,0.001);
    float ldn=dot(Ln,Nn);
    ldn = max(ldn,0.0);
    float ndv = dot(Nn,Vn);
    float hdv = dot(Hn,Vn);
    float eSq = Eccentricity*Eccentricity;
    float distrib = eSq / (rdv * rdv * (eSq - 1.0) + 1.0);
    distrib = distrib * distrib;
    float Gb = 2.0 * hdn * ndv / hdv;
    float Gc = 2.0 * hdn * ldn / hdv;
    float Ga = min(1.0,min(Gb,Gc));
    float fresnelHack = 1.0 - pow(ndv,5.0);
    hdn = distrib * Ga * fresnelHack / ndv;
    DiffuseContrib = ldn * LightColor;
    SpecularContrib = hdn * Ks * LightColor;
}

float4 std_PS(vertexOutput IN) : COLOR {
    float3 diffContrib=0;
    float3 specContrib=0;
    float3 ambContrib=0;
    float3 Ln = normalize(IN.LightVec);
    float3 Vn = normalize(IN.WorldView);
    float3 Nn = normalize(IN.WorldNormal);
	float3 Hn = normalize(Ln+Vn);
	float4 l = lit(dot(Nn, Ln), dot(Nn, Hn), 100);
	//blinn_shading(IN,Lamp0Color[0],Nn,Ln,Vn,diffContrib,specContrib);
    float3 diffuseColor = tex2D(ColorSampler,IN.UV).rgb;
	ambContrib = l.x;
	diffContrib = l.y;
	specContrib = l.z;
    float3 result = specContrib+(diffuseColor*(diffContrib+AmbiColor*ambContrib));
    // return as float4
    return float4(result,1);
}


/****************************************************************
	Shadow Caster Rendering
****************************************************************/
struct castOutput
{
    float4 HPosition	: POSITION;
    float4 Depth		: TEXCOORD0;
};

float4 camDepth : CAMERADEPTH;

castOutput CAST_VS(appdata IN) 
{
    castOutput OUT = (castOutput)0;

	// todo: support more than 1 bone
	OUT.HPosition = doSkinning(IN.Position, IN.BlendIdx, IN.BlendWeight / 255);
    OUT.HPosition = mul(OUT.HPosition,ViewProj); // ViewProj in this case is in light space
	OUT.Depth.x = OUT.HPosition.z / camDepth.x;

    return OUT;
}

float4 CAST_PS(castOutput IN) : COLOR 
{
    return float4(IN.Depth.x, IN.Depth.x, IN.Depth.x, 1);
}

/****************************************************************
	Shadow Receiver Rendering
****************************************************************/
sampler2D shadowTex;
float4x4 LightViewProj[1] : LIGHTVIEWPROJ;

struct recvOutput
{
    float4 HPosition		: POSITION;
    float2 UV				: TEXCOORD0;
    // The following values are passed in "World" coordinates since
    //   it tends to be the most flexible and easy for handling
    //   reflections, sky lighting, and other "global" effects.
    float3 LightVec			: TEXCOORD1;
    float3 WorldNormal		: TEXCOORD2;
    float3 WorldTangent		: TEXCOORD3;
    float3 WorldBinormal 	: TEXCOORD4;
    float3 WorldView		: TEXCOORD5;
    float4 LightSpacePos	: TEXCOORD6;
};

recvOutput RECV_VS(appdata IN) 
{
    recvOutput OUT = (recvOutput)0;

    OUT.UV = float2(IN.UV.x,(1.0-IN.UV.y));

	float oo255 = 1.0f / 255.0f;
	// todo: support more than 1 bone
	float4 oPos = doSkinning(IN.Position, IN.BlendIdx, IN.BlendWeight*oo255);

    float3 Pw = oPos.xyz;
    OUT.LightVec = (Lamp0Pos[0] - Pw);
    OUT.WorldView = normalize(ViewIXf[3].xyz - Pw);
    OUT.HPosition = mul(oPos,ViewProj);

	// since the world matrix is in the bones array, and we normally would want to multiply the 
	// normal by the WorldInvTrans, we need to change the order of the mul() operands (which effects
	// the transpose on the bone matrices) and multiply against the WorldInv (which will give us the WorldInvTrans
	// we're looking for)
	// todo: support more than one bone
	float4 worldNormal = doSkinning(IN.Normal, IN.BlendIdx, IN.BlendWeight*oo255);
    OUT.WorldNormal = mul(WorldInv, worldNormal).xyz;
	normalize(OUT.WorldNormal);

	// need to do the same for the tangent/binormal
	//float4 worldTangent = doSkinning(IN.Tangent, IN.BlendIdx, IN.BlendWeight*oo255);
    //OUT.WorldTangent = mul(WorldInv, worldTangent).xyz;
	//normalize(OUT.WorldTangent);

   // OUT.WorldBinormal = cross(OUT.WorldTangent, OUT.WorldNormal);
	
	// figure out vertex pos in light space
	OUT.LightSpacePos = mul(oPos, LightViewProj[0]);

    return OUT;
}

float4 RECV_PS(recvOutput IN) : COLOR 
{
    float3 diffContrib=0;
    float3 specContrib=0;
    float3 ambContrib=1;
    float3 diffuseColor = tex2D(ColorSampler,IN.UV).rgb;
	
	float2 texc = 0.5 * IN.LightSpacePos.xy / IN.LightSpacePos.w + float2(0.5,0.5);
	texc.y = 1 - texc.y;
	float shadow = tex2D(shadowTex, texc).r;

	if ((shadow+0.001) > (IN.LightSpacePos.z / camDepth.x))
	{
		float3 Ln = normalize(IN.LightVec);
		float3 Vn = normalize(IN.WorldView);
		float3 Nn = normalize(IN.WorldNormal);
		float3 Hn = normalize(Ln+Vn);
		float4 l = lit(dot(Nn, Ln), dot(Nn, Hn), 100);

		ambContrib = l.x;
		diffContrib = l.y;
		specContrib = l.z;
	}
	
    float3 result = specContrib+(diffuseColor*(diffContrib+AmbiColor*ambContrib));

    // return as float4
    return float4(result,1);
}

technique Main 
{
    pass p0
	{
        VertexShader = compile vs_3_0 std_VS();
        PixelShader = compile ps_3_0 std_PS();
    }
}

technique Main_ShadowCaster
{
    pass p0
	{
        VertexShader = compile vs_3_0 CAST_VS();
        PixelShader = compile ps_3_0 CAST_PS();
    }
}

technique Main_ShadowReceiver
{
    pass p0
	{
        VertexShader = compile vs_3_0 RECV_VS();
        PixelShader = compile ps_3_0 RECV_PS();
    }
}
