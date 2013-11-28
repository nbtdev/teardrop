/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(ATTRIBUTE_INCLUDED)
#define ATTRIBUTE_INCLUDED

#include "Memory/Allocators.h"

namespace Teardrop
{
	namespace Gfx 
	{
		class MaterialExpression;

		enum AttributeType {
			ATTR_UNKNOWN,
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
			ATTR_RGBA = ATTR_FLOAT4,
			ATTR_RGB = ATTR_FLOAT3,
		};

		struct Attribute {
			const char* mName;
			AttributeType mType;
			MaterialExpression* mParent;

			Attribute();
			Attribute(const char* name, AttributeType type, MaterialExpression* parent);

			static const char* paramTypeToString(AttributeType type);

			TD_DECLARE_ALLOCATOR();
		};
	}
}

#endif // ATTRIBUTE_INCLUDED
