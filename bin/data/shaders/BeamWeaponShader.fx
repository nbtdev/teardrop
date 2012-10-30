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

//ParticleShader.fx
struct PS_IN
{
	float4 hPos : POSITION;
	float2 texCoord : TEXCOORD0;
};

struct VS_IN
{
	float4 pos : POSITION;
	float2 texCoord : TEXCOORD0;
};

float4x4 WVP : WORLDVIEWPROJ;
float4 mountTransform[4];

PS_IN VS_MAIN(VS_IN vsIn)
{
	PS_IN psIn;
	
	float3 pos;
	pos.x = dot(mountTransform[0], vsIn.pos);
	pos.y = dot(mountTransform[1], vsIn.pos);
	pos.z = dot(mountTransform[2], vsIn.pos);

	psIn.hPos = mul(float4(pos, 1), WVP);
	psIn.texCoord = vsIn.texCoord;

	return psIn;
}

sampler2D tex;
float4 emissive : EMISSIVECOL;

float4 PS_MAIN(PS_IN psIn) : COLOR0
{
	//return tex2D(tex, psIn.texCoord);
	return emissive;
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
