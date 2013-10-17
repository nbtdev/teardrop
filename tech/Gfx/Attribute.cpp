/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "Attribute.h"

using namespace Teardrop;
using namespace Gfx;

Attribute::Attribute()
	: mName(0)
	, mType(ATTR_UNKNOWN)
	, mParent(0)
{

}

Attribute::Attribute(const char* name, AttributeType type, MaterialExpression* parent)
	: mName(name)
	, mType(type)
	, mParent(parent)
{

}
