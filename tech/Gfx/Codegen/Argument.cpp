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

#include "Argument.h"
#include "ShaderRealizer.h"

namespace Teardrop {
namespace Gfx {
namespace Codegen {

Argument::Argument(const String& aName, Direction aDirection, Expression::ConstPtr aExpression)
	: mName(aName)
	, mDirection(aDirection)
	, mExpression(aExpression)
{
}

Argument::Argument(Direction aDirection, AttributeExpression::ConstPtr aExpression)
	: Argument(aExpression->attrName(), aDirection, aExpression)
{
	mType = aExpression->attrType();
}

Argument::~Argument()
{
}

void Argument::realize(ShaderRealizer& aRealizer) const
{
	aRealizer.insertModifier(mDirection);
	mExpression->realize(aRealizer);
}

const String& Argument::name() const
{
	return mName;
}

Direction Argument::direction() const
{
	return mDirection;
}

DataType Argument::type() const
{
	return mType;
}

Expression::ConstRef Argument::expression() const 
{
	return mExpression;
}

} // namespace Codegen
} // namespace Gfx
} // namespace Teardrop
