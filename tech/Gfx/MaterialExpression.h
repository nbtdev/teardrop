/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(MATERIALEXPRESSION_INCLUDED)
#define MATERIALEXPRESSION_INCLUDED

#include "Reflection/Reflection.h"
#include "Memory/Allocators.h"
#include "Gfx/Attribute.h"
#include <iosfwd>
#include <string>
#include <vector>

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
				SHADER_HLSL,
				SHADER_HLSL5,
				SHADER_GLSL4,
				SHADER_GLSL_ES2,
			};

			// code generation
			void appendDefinition(Language lang, std::ostream& o);

			struct OutputName {
				Attribute* mOutputAttr;
				std::string mOutputVarName;
			};

			void appendCall(
				Language lang, 
				int ordinal, // instance ordinal for this expression in the shader
				const std::vector<std::string>& inputs, // input argument names
				std::vector<OutputName>& outputs, // generated output argument names
				std::ostream& o
				);

			typedef std::vector<Attribute> Attributes;
			const Attributes& inputAttributes();
			const Attributes& outputAttributes();

			TD_DECLARE_ALLOCATOR();

		protected:
			Attributes mInputs;
			Attributes mOutputs;

			virtual void appendBody(Language lang, std::ostream& o);
		};
	}
}

#endif // MATERIALEXPRESSION_INCLUDED
