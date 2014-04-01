/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/


#include "Sampler2DExpression.h"
#include "Sampler2D.h"

using namespace Teardrop;
using namespace Gfx;

TD_CLASS_IMPL(Sampler2DExpression);

Sampler2DExpression::Sampler2DExpression()
{
}

Sampler2DExpression::~Sampler2DExpression()
{
}

bool Sampler2DExpression::initialize()
{
	mInputs.push_back(Attribute("texcoord", ATTR_FLOAT2, this, Attribute::Optional, "psin.TXC0"));
	mOutputs.push_back(Attribute("RGBA", ATTR_RGBA, this));
	mOutputs.push_back(Attribute("RGB", ATTR_RGB, this));

	return true;
}

void Sampler2DExpression::appendBody(Language lang, std::ostream& o)
{
	switch (lang) {
	case SHADER_HLSL:
	case SHADER_HLSL5:
		o << "    RGBA = tex2D(" << mSamplerName << ", texcoord);" << std::endl;
		o << "    RGB = float3(RGBA.r, RGBA.g, RGBA.b);" << std::endl;
		break;
	default:
		break;
	}
}

void Sampler2DExpression::insertDependencies(Language lang, std::ostream& o)
{
	Sampler2D& samp = getSampler2D();
	if (samp.texture()) {
		// name only needs to be unique long enough to generate the shader(s)
		if (mSamplerName.length() == 0) {
			String tmp;
			StringUtil::toString((void*)&samp, tmp);
			mSamplerName = "samp2D_";
			mSamplerName.append(tmp);
		}

		switch (lang) {
		case SHADER_HLSL:
		case SHADER_HLSL5:
			o << "sampler2D " << mSamplerName << ";" << std::endl;
			break;
		default:
			break;
		}
	}
}