/******************************************************************************
Copyright (c) 2015 Teardrop Games

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


#include "AttributeExpression.h"
#include "Attribute.h"
#include "Common.h"
#include "ShaderRealizer.h"

namespace {
	using namespace Teardrop::Gfx::Codegen;

	DataType sTypeLut[] = {
		TYPE_UNKNOWN,		// ATTR_UNKNOWN
		TYPE_UNKNOWN,		// ATTR_INT
		TYPE_UNKNOWN,		// ATTR_INT2
		TYPE_UNKNOWN,		// ATTR_INT3
		TYPE_UNKNOWN,		// ATTR_INT4
		TYPE_FLOAT,			// ATTR_FLOAT
		TYPE_FLOAT2,		// ATTR_FLOAT2
		TYPE_FLOAT3,		// ATTR_FLOAT3
		TYPE_FLOAT4,		// ATTR_FLOAT4
		TYPE_UNKNOWN,		// ATTR_MAT22
		TYPE_UNKNOWN,		// ATTR_MAT33
		TYPE_UNKNOWN,		// ATTR_MAT34
		TYPE_UNKNOWN,		// ATTR_MAT44
		TYPE_UNKNOWN,		// ATTR_MAT43
		TYPE_FLOAT4,		// ATTR_RGBA = ATTR_FLOAT4
		TYPE_FLOAT3,		// ATTR_RGB = ATTR_FLOAT3
	};
} // namespace

namespace Teardrop {
namespace Gfx {
namespace Codegen {

AttributeExpression::AttributeExpression(const Gfx::Attribute& aAttr)
	: mAttr(aAttr)
{

}

AttributeExpression::~AttributeExpression()
{

}

const char* AttributeExpression::attrName() const
{
	return mAttr.mName;
}

DataType AttributeExpression::attrType() const
{
	return sTypeLut[mAttr.mType];
}

void AttributeExpression::realize(ShaderRealizer& aRealizer) const
{
	aRealizer.insertDataType(sTypeLut[mAttr.mType]);
	aRealizer.insertIdentifier(mAttr.mName);
}

} // namespace Codegen
} // namespace Gfx
} // namespace Teardrop
