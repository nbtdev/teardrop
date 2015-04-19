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

#if !defined(TEARDROP_GFX_CODEGEN_ATTRIBUTE_INCLUDED)
#define TEARDROP_GFX_CODEGEN_ATTRIBUTE_INCLUDED

#include "Gfx/Codegen/AttributeExpression.h"
#include "Gfx/Codegen/Common.h"
#include "Gfx/Codegen/Expression.h"
#include "Util/_String.h"
#include <memory>

namespace Teardrop
{
	namespace Gfx
	{
		namespace Codegen
		{
			class Argument : public Expression
			{
			public:
				Argument(Direction aDirection, AttributeExpression::ConstPtr aExpr);
				Argument(const String& aName, Direction aDirection, Expression::ConstPtr aExpr);
				~Argument();

				void realize(ShaderRealizer& aRealizer) const;

				const String& name() const;
				Direction direction() const;
				DataType type() const;
				Expression::ConstRef expression() const;

				typedef std::shared_ptr<Argument> Ptr;
				typedef std::shared_ptr<const Argument> ConstPtr;
				typedef std::weak_ptr<Argument> Ref;
				typedef std::weak_ptr<const Argument> ConstRef;

			protected:
				String mName;
				Direction mDirection = DIRECTION_IN;
				DataType mType = TYPE_UNKNOWN;
				Expression::ConstPtr mExpression;
			};
		} // namespace Codegen
	} // namespace Gfx
} // namespace Teardrop

#endif // TEARDROP_GFX_CODEGEN_ATTRIBUTE_INCLUDED
