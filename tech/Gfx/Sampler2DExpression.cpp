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
