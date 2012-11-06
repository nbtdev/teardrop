/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(STRINGUTIL_INCLUDED)
#define STRINGUTIL_INCLUDED

namespace Teardrop
{
	class String;
	class Transform;
	class Vector4;
	class Vector2;
	class Quaternion;

	struct StringUtil
	{
		static void toString(float c, String& str);
		static void toString(float c, char* buf, int size);
		static void toString(bool c, String& str);
		static void toString(bool c, char* buf, int size);
		static void toString(char c, String& str);
		static void toString(char c, char* buf, int size);
		static void toString(unsigned char c, String& str);
		static void toString(unsigned char c, char* buf, int size);
		static void toString(short c, String& str);
		static void toString(short c, char* buf, int size);
		static void toString(unsigned short c, String& str);
		static void toString(unsigned short c, char* buf, int size);
		static void toString(int c, String& str);
		static void toString(int c, char* buf, int size);
		static void toString(unsigned int c, String& str);
		static void toString(unsigned int c, char* buf, int size);
		static void toString(long c, String& str);
		static void toString(long c, char* buf, int size);
		static void toString(unsigned long c, String& str);
		static void toString(unsigned long c, char* buf, int size);
		static void toString(void* c, String& str);
		static void toString(void* c, char* buf, int size);

		static void fromString(const String& str, float& c);
		static void fromString(const char* buf, float& c);
		static void fromString(const String& str, bool& c);
		static void fromString(const char* buf, bool& c);
		static void fromString(const String& str, char& c);
		static void fromString(const char* buf, char& c);
		static void fromString(const String& str, unsigned char& c);
		static void fromString(const char* buf, unsigned char& c);
		static void fromString(const String& str, short& c);
		static void fromString(const char* buf, short& c);
		static void fromString(const String& str, unsigned short& c);
		static void fromString(const char* buf, unsigned short& c);
		static void fromString(const String& str, int& c);
		static void fromString(const char* buf, int& c);
		static void fromString(const String& str, unsigned int& c);
		static void fromString(const char* buf, unsigned int& c);
		static void fromString(const String& str, long& c);
		static void fromString(const char* buf, long& c);
		static void fromString(const String& str, unsigned long& c);
		static void fromString(const char* buf, unsigned long& c);
		static void fromString(const String& str, void*& c);
		static void fromString(const char* buf, void*& c);

		// this seems pedantic, but Reflection wants it for completeness,
		// and it also makes serialization to/from XML simpler
		static void fromString(const String& in, String& out);
		static void toString(const String& in, String& out);

		// additional conversion functions for use by Reflection
		static void fromString(const String& in, Transform& out);
		static void toString(const Transform& in, String& out);
		static void fromString(const String& in, Vector4& out);
		static void toString(const Vector4& in, String& out);
		static void fromString(const String& in, Vector2& out);
		static void toString(const Vector2& in, String& out);
		static void fromString(const String& in, Quaternion& out);
		static void toString(const Quaternion& in, String& out);

		// convert from DOS (\) to UNIX (/) path separators
		static void toUNIXPath(/*inout*/ String& s);
	};
}

#endif // STRINGUTIL_INCLUDED
