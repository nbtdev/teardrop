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
			const char* mDefault;
			AttributeType mType;
			MaterialExpression* mParent;
			bool mRequired;

			static const bool Required = true;
			static const bool Optional = false;

			Attribute();
			Attribute(const char* name, AttributeType type, MaterialExpression* parent, bool required = true, const char* defaultValue = 0);

			static const char* paramTypeToString(AttributeType type);

			TD_DECLARE_ALLOCATOR();
		};
	}
}

#endif // ATTRIBUTE_INCLUDED
