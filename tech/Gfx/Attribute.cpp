/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/


#include "Attribute.h"

using namespace Teardrop;
using namespace Gfx;

Attribute::Attribute()
	: mName(0)
	, mType(ATTR_UNKNOWN)
	, mParent(0)
	, mRequired(Optional)
	, mDefault(0)
{

}

Attribute::Attribute(const char* name, AttributeType type, MaterialExpression* parent, bool required, const char* defaultValue)
	: mName(name)
	, mType(type)
	, mParent(parent)
	, mRequired(required)
	, mDefault(defaultValue)
{

}

static const char* sAttrNameLut[] = {
	"void",		//ATTR_UNKNOWN,
	"int",		//ATTR_INT,
	"int2",		//ATTR_INT2,
	"int3",		//ATTR_INT3,
	"int4",		//ATTR_INT4,
	"float",	//ATTR_FLOAT,
	"float2",	//ATTR_FLOAT2,
	"float3",	//ATTR_FLOAT3,
	"float4",	//ATTR_FLOAT4,
	"mat22",	//ATTR_MAT22,
	"mat33",	//ATTR_MAT33,
	"mat34",	//ATTR_MAT34,
	"mat44",	//ATTR_MAT44,
	"mat43",	//ATTR_MAT43,
	"RGBA",		//ATTR_RGBA = ATTR_FLOAT4,
	"RGB",		//ATTR_RGB = ATTR_FLOAT3,
};

const char* Attribute::paramTypeToString(AttributeType type)
{
	return sAttrNameLut[type];
}