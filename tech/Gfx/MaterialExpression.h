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
				int ordinal, // instance ordinal for this expression in the shader
				const std::vector<std::string>& inputs, // input argument names
				const std::map<const Attribute*, std::string>& outputs, // output argument names 
				std::ostream& o
				);

			typedef std::vector<Attribute> Attributes;
			const Attributes& inputAttributes();
			const Attributes& outputAttributes();
			const ShaderFeatures& features();

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
		};
	}
}

#endif // MATERIALEXPRESSION_INCLUDED
