/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(VARIANT_INCLUDED)
#define VARIANT_INCLUDED

namespace Teardrop
{
	struct Variant
	{
		typedef enum
		{
			VT_NONE,
			VT_INT,
			VT_BOOL,
			VT_FLOAT,
			VT_STRING,
			VT_USER
		} Type;

		union Val
		{
			int i;
			float f;
			const char* s;
			void* p;
			bool b;
		};

		Val v;
		Type type;

		void setInt(int i);
		void setFloat(float f);
		void setString(const char* s);
		void setBool(bool b);
		void setUser(void* p);
	};
}

#endif // VARIANT_INCLUDED
