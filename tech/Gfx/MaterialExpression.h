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

#if !defined(MATERIALEXPRESSION_INCLUDED)
#define MATERIALEXPRESSION_INCLUDED

#include "Reflection/Reflection.h"
#include "Memory/Allocators.h"
#include "Gfx/Attribute.h"
#include "Gfx/ShaderFeatures.h"
#include <iosfwd>
#include <string>
#include <vector>
#include <map>

namespace Teardrop
{
	namespace Gfx 
	{
		class MaterialExpression : public Reflection::Object
		{
		public:
			TD_CLASS(MaterialExpression, Object);

			MaterialExpression();
			~MaterialExpression();

			Attribute* findInputAttribute(const char* name);
			Attribute* findOutputAttribute(const char* name);
			
			enum Language {
				SHADER_HLSL5,
				SHADER_GLSL4,
				SHADER_GLSL_ES2,
			};

			// code generation
			// aSampIndex is used by Sampler* expressions; this parameter is passed through to the
			// insertDependencies protected method below
			void appendDefinition(Language lang, std::ostream& o);

			void appendCall(
				Language lang, 
                size_t ordinal, // instance ordinal for this expression in the shader
				const std::vector<std::string>& inputs, // input argument names
				const std::map<const Attribute*, std::string>& outputs, // output argument names 
				std::ostream& o
				);

			typedef std::vector<Attribute> Attributes;
			const Attributes& inputAttributes();
			const Attributes& outputAttributes();
			const ShaderFeatures& features();
            uint64_t hash(uint64_t seed);

			TD_DECLARE_ALLOCATOR();

		protected:
			Attributes mInputs;
			Attributes mOutputs;
			ShaderFeatures mFeatures;

			virtual void appendBody(Language lang, std::ostream& o);

			// as there may be more than one sampler used in
			// a Material, Sampler* expressions should (if necessary) use 
			// the value of aSampIndex passed to the method, 
			// and increment aSampIndex before returning
			virtual void insertDependencies(Language lang, std::ostream& o);

			// allow subclasses to provide a custom name for their functions (defaults to the 
			// ClassDef name)
			virtual void insertFunctionName(Language lang, std::ostream& o);

            // allow subclasses to hash their static inputs into the hash provided by "seed"
            virtual uint64_t hashStatics(uint64_t seed);
		};
	}
}

#endif // MATERIALEXPRESSION_INCLUDED
