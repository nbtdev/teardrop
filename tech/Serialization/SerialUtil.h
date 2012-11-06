/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SERIALUTIL_INCLUDED)
#define SERIALUTIL_INCLUDED

namespace Teardrop
{
	class Stream;
	class String;

	namespace Serial
	{
		//
		// helper free-functions for serialization
		//
		struct Util
		{
			static bool write(char c, Stream& stream);
			static bool write(unsigned char c, Stream& stream);
			static bool write(short s, Stream& stream);
			static bool write(unsigned short s, Stream& stream);
			static bool write(int s, Stream& stream);
			static bool write(unsigned int s, Stream& stream);
			static bool write(long s, Stream& stream);
			static bool write(unsigned long s, Stream& stream);
			static bool write(const char* p, Stream& stream);
			static bool write(const String& s, Stream& stream);
			static bool read(char& c, Stream& stream);
			static bool read(unsigned char& c, Stream& stream);
			static bool read(short& s, Stream& stream);
			static bool read(unsigned short& s, Stream& stream);
			static bool read(int& s, Stream& stream);
			static bool read(unsigned int& s, Stream& stream);
			static bool read(long& s, Stream& stream);
			static bool read(unsigned long& s, Stream& stream);
			static bool read(char* p, Stream& stream, size_t len);
			static bool read(String& s, Stream& stream);

			static bool verifyPlatformDataSizes();
			static void mapResourceDependencyName(const char* src, char* dest, size_t destLen);
		};
	}
}

#endif // SERIALUTIL_INCLUDED
