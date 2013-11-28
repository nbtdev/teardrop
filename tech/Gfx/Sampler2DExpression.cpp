/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/


#include "Sampler2DExpression.h"

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
	mOutputs.push_back(Attribute("RGBA", ATTR_RGBA, this));
	mOutputs.push_back(Attribute("RGB", ATTR_RGB, this));

	return true;
}

static const char* HLSL_BODY =
"    c = samp(texcoord);\n"
;

void Sampler2DExpression::appendBody(Language lang, std::ostream& o)
{
	switch (lang) {
	case SHADER_HLSL:
	case SHADER_HLSL5:
		o << HLSL_BODY;
		break;
	default:
		break;
	}
}
