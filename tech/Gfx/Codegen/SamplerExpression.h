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

#if !defined(TEARDROP_GFX_CODEGEN_SAMPLEREXPRESSION_INCLUDED)
#define TEARDROP_GFX_CODEGEN_SAMPLEREXPRESSION_INCLUDED

#include "Gfx/Codegen/Expression.h"
#include "Util/_String.h"

namespace Teardrop
{
	namespace Gfx
	{
		class Sampler2D;

		namespace Codegen
		{
			class SamplerExpression : public Expression
			{
			public:
				SamplerExpression(const String& aName, const Sampler2D* aSampler2D, Expression::ConstPtr aTexcoord);
				~SamplerExpression();

				void realize(ShaderRealizer& aRealizer) const;

				const String& name() const;
				Expression::ConstRef texcoord() const;

				typedef std::shared_ptr<SamplerExpression> Ptr;
				typedef std::shared_ptr<const SamplerExpression> ConstPtr;
				typedef std::weak_ptr<SamplerExpression> Ref;
				typedef std::weak_ptr<const SamplerExpression> ConstRef;

				TD_DECLARE_ALLOCATOR()

			protected:
				String mName;
				const Sampler2D* mSampler2D = nullptr;
				Expression::ConstPtr mTexcoord;

			private:
			};

		} // namespace Codegen
	} // namespace Gfx
} // namespace Teardrop

#endif // TEARDROP_GFX_CODEGEN_SAMPLEREXPRESSION_INCLUDED
