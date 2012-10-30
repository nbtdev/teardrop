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

#define FLIP_TEXTURE_Y
float4x4 WorldITXf : WORLDINVTRANS;
float4x4 WvpXf : WORLDVIEWPROJ;
float4x4 WorldXf : WORLD;
float4x4 ViewIXf : VIEWINV;
float4 camDepth : CAMERADEPTH;

float3 Lamp0Pos[1] : LIGHTPOS;
float3 Lamp0Color[1] : LIGHTCOL;
float3 AmbiColor : WORLDAMBIENT;
float Ks = 0.2;
float Eccentricity = 0.05;
sampler2D ColorSampler;
sampler2D AOSampler;

/***************************************************************************
	Standard rendering
***************************************************************************/

struct appdata {
    float4 Position	: POSITION;
    float4 UV		: TEXCOORD0;
    float4 Normal	: NORMAL;
};

struct vertexOutput {
    float4 HPosition	: POSITION;
    float2 UV		: TEXCOORD0;
    // The following values are passed in "World" coordinates since
    //   it tends to be the most flexible and easy for handling
    //   reflections, sky lighting, and other "global" effects.
    float3 LightVec	: TEXCOORD1;
    float3 WorldNormal	: TEXCOORD2;
    float3 WorldView	: TEXCOORD5;
};

vertexOutput std_VS(appdata IN) {
    vertexOutput OUT = (vertexOutput)0;
    OUT.WorldNormal = mul(IN.Normal,WorldITXf).xyz;
    float4 Po = IN.Position;
    float3 Pw = mul(Po,WorldXf).xyz;
    OUT.LightVec = (Lamp0Pos[0] - Pw);

    OUT.UV = float2(IN.UV.x,(1.0-IN.UV.y));

    OUT.WorldView = normalize(ViewIXf[3].xyz - Pw);
    OUT.HPosition = mul(Po,WvpXf);
    return OUT;
}

// Utility function for blinn shading

void blinn_shading(
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
    float3 diffContrib;
    float3 specContrib;
    float3 Ln = normalize(IN.LightVec);
    float3 Vn = normalize(IN.WorldView);
    float3 Nn = normalize(IN.WorldNormal);
	blinn_shading(Lamp0Color[0],Nn,Ln,Vn,diffContrib,specContrib);
    float3 diffuseColor = tex2D(ColorSampler,IN.UV).rgb;
    float3 aoColor = tex2D(AOSampler,IN.UV).rgb;
    float3 result = aoColor * (specContrib+(diffuseColor*(diffContrib+AmbiColor)));
    // return as float4
    return float4(result,1);
}

/***************************************************************************
	Shadow receiver rendering
***************************************************************************/
sampler2D shadowTex;
float4x4 LightViewProj[1] : LIGHTVIEWPROJ;

struct recvPSIN {
    float4 HPosition	: POSITION;
    float2 UV		: TEXCOORD0;
    // The following values are passed in "World" coordinates since
    //   it tends to be the most flexible and easy for handling
    //   reflections, sky lighting, and other "global" effects.
    float3 LightVec	: TEXCOORD1;
    float3 WorldNormal	: TEXCOORD2;
    float3 WorldView	: TEXCOORD5;
	float4 PosLightSpace : TEXCOORD6;
};

recvPSIN receiver_VS(appdata IN) {
    recvPSIN OUT = (recvPSIN)0;
    OUT.WorldNormal = mul(IN.Normal,WorldXf).xyz;
    float4 Po = IN.Position;
    float3 Pw = mul(Po,WorldXf).xyz;
    OUT.LightVec = (Lamp0Pos[0] - Pw);

    OUT.UV = float2(IN.UV.x,(1.0-IN.UV.y));

    OUT.WorldView = normalize(ViewIXf[3].xyz - Pw);
    OUT.HPosition = mul(Po,WvpXf);
	
	// get depth in light space
	float4x4 wvpLightSpace = mul(WorldXf, LightViewProj[0]);
	OUT.PosLightSpace = mul(Po, wvpLightSpace);
    return OUT;
}

float4 receiver_PS(recvPSIN IN) : COLOR {
    float3 diffContrib = 0;
    float3 specContrib = 0;
	float3 ambContrib = 1;

    float3 diffuseColor = tex2D(ColorSampler,IN.UV).rgb;
    float3 aoColor = tex2D(AOSampler,IN.UV).rgb;

	float2 texc = 0.5 * IN.PosLightSpace.xy / IN.PosLightSpace.w + float2(0.5,0.5);
	texc.y = 1 - texc.y;
	float shadow = tex2D(shadowTex, texc).r;

	if ((shadow+0.001) > (IN.PosLightSpace.z / camDepth.x))
	{
		float3 L = normalize(IN.LightVec);
		float3 N = normalize(IN.WorldNormal);
		float3 V = normalize(IN.WorldView);
		float3 H = normalize(L+V);
		float NdotH = dot(N, H);
		float4 l = lit(dot(N, L), NdotH, 50);
		
		ambContrib = l.x;
		diffContrib = l.y;
		specContrib = l.z * Ks;
	}

	float3 result = aoColor * (specContrib + (diffuseColor * (AmbiColor * ambContrib + diffContrib)));

    // return as float4
    return float4(result,1);  
}

/***************************************************************************
	Shadowcaster rendering
***************************************************************************/

struct VS_IN {
    float4 Position	: POSITION;
};

struct PS_IN {
    float4 HPosition	: POSITION;
    float4  Depth	: TEXCOORD0;
};

PS_IN VS_CAST(VS_IN IN) {
    PS_IN OUT = (PS_IN)0;
    OUT.HPosition = mul(IN.Position, WvpXf);
	OUT.Depth.x = OUT.HPosition.z / camDepth.x;
    return OUT;
}

float4 PS_CAST(PS_IN IN) : COLOR 
{
    return float4(IN.Depth.x, IN.Depth.x, IN.Depth.x, 1);
}

/***************************************************************************
	Techniques
***************************************************************************/

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
        VertexShader = compile vs_3_0 VS_CAST();
        PixelShader = compile ps_3_0 PS_CAST();
    }
}

technique Main_ShadowReceiver
{
    pass p0
	{
        VertexShader = compile vs_3_0 receiver_VS();
        PixelShader = compile ps_3_0 receiver_PS();
    }
}

