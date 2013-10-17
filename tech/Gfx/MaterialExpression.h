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

			TD_DECLARE_ALLOCATOR();

		protected:
			typedef std::vector<Attribute> Attributes;
			Attributes mInputs;
			Attributes mOutputs;
		};
	}
}

#endif // MATERIALEXPRESSION_INCLUDED
