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

//TerrainShader.fx
struct PS_IN
{
	float4 hPos : POSITION;
	float2 texDiffuse : TEXCOORD0;
	float2 texDetail : TEXCOORD1;
	float3 normal : TEXCOORD2;
};

struct VS_IN
{
	float3 pos : POSITION;
	//float4 norm : NORMAL;
};

float xStep;
float zStep;
float uStart;
float vStart;
float uStep;
float vStep;
float xScale;
float zScale;

float4x4 WorldViewProj : WORLDVIEWPROJ;
float4 LightPos[4] : LIGHTPOS;

PS_IN VS_MAIN(VS_IN vsIn)
{
	PS_IN psIn;

	float4 pos = float4(vsIn.pos, 1);
	pos.x *= xStep;
	pos.z *= zStep;

	float2 uvStart = {uStart, vStart};
	float2 uvStep = {uStep, vStep};
	float2 uvDiffuse = vsIn.pos.xz * uvStep + uvStart;

	psIn.texDiffuse = uvDiffuse;
	psIn.texDetail = pos.xz;

	pos.x *= xScale;
	pos.z *= zScale;

	psIn.hPos = mul(pos, WorldViewProj);
	psIn.normal = LightPos[0].xyz;
	return psIn;
}

sampler2D DiffTex;
sampler2D DetailTex;

float4 PS_MAIN(PS_IN psIn) : COLOR0
{
	float4 oCol = tex2D(DiffTex, psIn.texDiffuse);
	oCol *= tex2D(DetailTex, psIn.texDetail);
	return oCol;
}



/***************************************************************************
	Shadowreceiver rendering
***************************************************************************/
float4x4 LightViewProj[1] : LIGHTVIEWPROJ;
float4x4 WorldXf : WORLD;
float4 camDepth : CAMERADEPTH;

struct PS_IN_RECV
{
	float4 hPos : POSITION;
	float2 texDiffuse : TEXCOORD0;
	float2 texDetail : TEXCOORD1;
	float3 normal : TEXCOORD2;
	float4 lightSpacePos : TEXCOORD3;
};

PS_IN_RECV VS_RECV(VS_IN vsIn)
{
	PS_IN_RECV psIn;

	float4 pos = float4(vsIn.pos, 1);
	pos.x *= xStep;
	pos.z *= zStep;

	float2 uvStart = {uStart, vStart};
	float2 uvStep = {uStep, vStep};
	float2 uvDiffuse = vsIn.pos.xz * uvStep + uvStart;

	psIn.texDiffuse = uvDiffuse;
	psIn.texDetail = pos.xz;

	pos.x *= xScale;
	pos.z *= zScale;

	psIn.hPos = mul(pos, WorldViewProj);
	psIn.normal = LightPos[0].xyz;

	// get depth in light space
	float4x4 wvpLightSpace = mul(WorldXf, LightViewProj[0]);
	psIn.lightSpacePos = mul(pos, wvpLightSpace);

	return psIn;
}

sampler2D ShadowMap;

float4 PS_RECV(PS_IN_RECV psIn) : COLOR0
{
	float4 oCol = tex2D(DiffTex, psIn.texDiffuse);
	oCol *= tex2D(DetailTex, psIn.texDetail);
	
	// hack for now -- just modulate into shadow if shadowed
	float2 texc = 0.5 * psIn.lightSpacePos.xy / psIn.lightSpacePos.w + float2(0.5,0.5);
	texc.y = 1 - texc.y;
	float shadow = tex2D(ShadowMap, texc).r;

	if (shadow < (psIn.lightSpacePos.z / camDepth.x + 0.05))
	{
		//oCol *= 0.8;
	}
	return oCol;
}



/***************************************************************************
	Shadowcaster rendering
***************************************************************************/

struct VSIN {
    float4 pos	: POSITION;
};

struct PSIN {
    float4 HPosition	: POSITION;
    float4  Depth	: TEXCOORD0;
};

PSIN VS_CAST(VSIN vsIn) {
	PSIN psIn = (PSIN)0;

	float4 pos = vsIn.pos;
	pos.x *= xStep;
	pos.z *= zStep;

	pos.x *= xScale;
	pos.z *= zScale;

	psIn.HPosition = mul(pos, WorldViewProj);
	psIn.Depth.x = psIn.HPosition.z / camDepth.x;
	return psIn;
}

float4 PS_CAST(PSIN IN) : COLOR 
{
    return float4(IN.Depth.x, 0, 0, 1);
}

/***************************************************************************
	Techniques
***************************************************************************/

technique Main
{
	pass P0
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
}


technique Main_ShadowCaster
{
    pass p0
	{
        VertexShader = compile vs_3_0 VS_CAST();
        PixelShader = compile ps_3_0 PS_CAST();
    }
}

technique Main_ShadowReceiver
{
    pass p0
	{
        VertexShader = compile vs_3_0 VS_RECV();
        PixelShader = compile ps_3_0 PS_RECV();
    }
}