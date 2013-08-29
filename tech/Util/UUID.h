/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(UTIL_UUID_INCLUDED)
#define UTIL_UUID_INCLUDED

namespace Teardrop
{
	class String;

	struct UUID
	{
		UUID();
		UUID(const UUID& other);
		~UUID();
		bool operator==(const UUID& other) const;
		bool operator!=(const UUID& other) const;
		bool operator<(const UUID& other) const;

		void generate();
		void toString(String& str) const;
		void fromString(const String& str);

		unsigned long long mLow;
		unsigned long long mHigh;
	};
}

#endif // UTIL_UUID_INCLUDED