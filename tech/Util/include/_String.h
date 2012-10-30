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

#if !defined(COS_STRING_INCLUDED)
#define COS_STRING_INCLUDED

#include "Serialization/include/SerialPointer.h"
#include "Serialization/include/Serialization.h"
#include "Memory/include/Memory.h"

namespace CoS
{
	class Allocator;
	class ResourceSerializer;
	class Stream;

	/*
		CoS String class, UTF-8 only, serializable
	*/
	class String
	{
		DECLARE_SERIALIZABLE(String);

		static const int DEFAULT_BUFLEN = 16;
		SerialPointer<char> m_pBuf;
		char m_default[DEFAULT_BUFLEN];
		unsigned int m_len;

		static Allocator* s_pAllocator;

	public:
		String();
		String(const String& other);
		String(const char* other);
		~String();

		static const unsigned int INVALID = 0xFFFFFFFF;
		static const String& EMPTY;
		static void setAllocator(Allocator* pAllocator);

		/*
			String operators
		*/
		String& operator=(const String& other);
		String& operator=(const char* other);
		bool operator==(const String& other) const;
		bool operator==(const char* other) const;
		bool operator!=(const String& other) const;
		bool operator!=(const char* other) const;
		String& operator+=(const String& other);
		String& operator+=(const char* other);
		bool operator<(const String& other) const; // for maps, etc
		operator const char*() const { return m_pBuf; }
		char& operator[](size_t idx);

		/*
			String methods
		*/
		String& append(const String& other);
		String& append(const char* other);
		size_t findFirst(char c) const;
		size_t findLast(char c) const;
		bool contains(const String& search) const;
		bool contains(const char* search) const;
		size_t length() const;
		void truncate(size_t len);
		void clear();

		/* NOTE: THESE RETURN COPIES */
		String substr(size_t begin=0, size_t end=INVALID) const;

		/*
			Serialization
		*/
		bool serialize(ResourceSerializer& serializer) const;
		bool serialize(Stream& strm) const;
		bool deserialize(Stream& strm);

		COS_DECLARE_ALLOCATOR();

	private:
		size_t _resize(size_t len);
	};

	/*
		Convenience operators -- note that these return COPIES
	*/
	String operator+(const String& l, const String& r);
	String operator+(const String& l, const char* r);
	String operator+(const char* l, const String& r);
}

#endif // COS_STRING_INCLUDED
