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
#include <vector>

namespace Teardrop
{
	namespace Gfx 
	{
		class MaterialExpression : public Reflection::Object
		{
		public:
			TD_CLASS(MaterialExpression, Object);
			TD_CLASS_CREATABLE();

			MaterialExpression();
			~MaterialExpression();

			enum AttributeType {
				ATTR_INT,
				ATTR_INT2,
				ATTR_INT3,
				ATTR_INT4,
				ATTR_FLOAT,
				ATTR_FLOAT2,
				ATTR_FLOAT3,
				ATTR_FLOAT4,
				ATTR_MAT22,
				ATTR_MAT33,
				ATTR_MAT34,
				ATTR_MAT44,
				ATTR_MAT43,
				ATTR_COLOR = ATTR_FLOAT4,
			};

			struct Attribute {
				const char* mName;
				AttributeType mType;
				Attribute();
				Attribute(const char* name, AttributeType type);
			};

			TD_DECLARE_ALLOCATOR();

		protected:
			typedef std::vector<Attribute> Attributes;
			Attributes mInputs;
			Attributes mOutputs;
		};
	}
}

#endif // MATERIALEXPRESSION_INCLUDED
