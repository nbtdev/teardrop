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

#if !defined(SAMPLER2DEXPRESSION_INCLUDED)
#define SAMPLER2DEXPRESSION_INCLUDED

#include "Gfx/MaterialExpression.h"
#include "Gfx/Sampler2D.h"

namespace Teardrop
{
	namespace Gfx 
	{
		class Sampler2DExpression : public MaterialExpression
		{
		public:
			TD_CLASS(Sampler2DExpression, MaterialExpression);
			TD_CLASS_CREATABLE();
			TD_NESTED_PROPERTY(Sampler2D, "2D Texture Sampler", Sampler2D);

			Sampler2DExpression();
			~Sampler2DExpression();

			bool initialize();
			const String& textureName();
			const String& samplerName();

			// custom method variant, not an override
			void appendDefinition(Language lang, int aTextureIndex, int aSamplerIndex, std::ostream& o);

			TD_DECLARE_ALLOCATOR();

		protected:
			// code generation
			void appendBody(Language lang, std::ostream& o);

			// custom method variant, not an override
			void insertDependencies(Language lang, int aTextureIndex, int aSamplerIndex, std::ostream& o);

			// custom function name
			void insertFunctionName(Language lang, std::ostream& o);

			String mTextureName;
			String mSamplerName;
		};
	}
}

#endif // SAMPLER2DEXPRESSION_INCLUDED
