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

#if !defined(TEARDROP_GFX_CODEGEN_FUNCTION_INCLUDED)
#define TEARDROP_GFX_CODEGEN_FUNCTION_INCLUDED

#include "Gfx/Codegen/Argument.h"
#include "Gfx/Codegen/Expression.h"
#include "Memory/Allocators.h"
#include "Util/_String.h"
#include <memory>
#include <vector>

namespace Teardrop
{
	namespace Gfx
	{
		namespace Codegen
		{
			class Argument;

			class Function : public Expression
			{
			public:
				Function(const String& aName);
				~Function();

				void realize(ShaderRealizer& aRealizer) const;

				void addArgument(Argument::ConstPtr aArg);
				void addStatement(Expression::ConstPtr aStatement);

				const String& name() const;
				unsigned int argumentCount() const;
				unsigned int statementCount() const;
				Argument::ConstPtr argument(unsigned int aIndex) const;
				Expression::ConstRef statement(unsigned int aIndex) const;

				typedef std::shared_ptr<Function> Ptr;
				typedef std::shared_ptr<const Function> ConstPtr;
				typedef std::weak_ptr<Function> Ref;
				typedef std::weak_ptr<const Function> ConstRef;

				TD_DECLARE_ALLOCATOR();

			protected:
				std::vector<Argument::ConstPtr> mArguments;
				std::vector<Expression::ConstPtr> mStatements;
				String mName;
			};
		} // namespace Codegen
	} // namespace Gfx
} // namespace Teardrop

#endif // TEARDROP_GFX_CODEGEN_FUNCTION_INCLUDED
