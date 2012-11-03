/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

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
-----------------------------------------------------------------------------
*/

#if !defined(SERIALUTIL_INCLUDED)
#define SERIALUTIL_INCLUDED

namespace CoS
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
