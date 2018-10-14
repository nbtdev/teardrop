/******************************************************************************
Copyright (c) 2018 Teardrop Games

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
******************************************************************************/


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
