/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "MaterialExpression.h"

using namespace Teardrop;
using namespace Gfx;

TD_CLASS_IMPL(MaterialExpression);

MaterialExpression::MaterialExpression()
	: mInputs(0)
	, mOutputs(0)
{
}

MaterialExpression::~MaterialExpression()
{
}

MaterialExpression::Attribute::Attribute()
{

}

MaterialExpression::Attribute::Attribute(const char* name, AttributeType type)
	: mName(name)
	, mType(type)
{

}
