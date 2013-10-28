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
			static int write(char c, Stream& stream);
			static int write(unsigned char c, Stream& stream);
			static int write(short s, Stream& stream);
			static int write(unsigned short s, Stream& stream);
			static int write(int s, Stream& stream);
			static int write(unsigned int s, Stream& stream);
			static int write(long s, Stream& stream);
			static int write(unsigned long s, Stream& stream);
			static int write(const char* p, Stream& stream);
			static int write(const String& s, Stream& stream);
			static int read(char& c, Stream& stream);
			static int read(unsigned char& c, Stream& stream);
			static int read(short& s, Stream& stream);
			static int read(unsigned short& s, Stream& stream);
			static int read(int& s, Stream& stream);
			static int read(unsigned int& s, Stream& stream);
			static int read(long& s, Stream& stream);
			static int read(unsigned long& s, Stream& stream);
			static int read(char* p, Stream& stream, size_t len);
			static int read(String& s, Stream& stream);

			static bool verifyPlatformDataSizes();
			static void mapResourceDependencyName(const char* src, char* dest, size_t destLen);
		};
	}
}

#endif // SERIALUTIL_INCLUDED
