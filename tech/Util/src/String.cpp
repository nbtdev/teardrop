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

#include "stdafx.h"
#include "_String.h"
#include <string.h>
#include "Stream/include/Stream.h"
#include "Memory/include/Memory.h"

using namespace CoS;
//---------------------------------------------------------------------------
const String& String::EMPTY = String();
//---------------------------------------------------------------------------
Allocator* String::s_pAllocator = 0; // USER MUST SET THIS
//---------------------------------------------------------------------------
void String::setAllocator(Allocator* pAlloc)
{
	s_pAllocator = pAlloc;
}
//---------------------------------------------------------------------------
String::String()
{
	m_default[0] = 0;
	m_pBuf = m_default;
	m_len = 0;
}
//---------------------------------------------------------------------------
String::~String()
{
	if (m_pBuf != m_default)
		s_pAllocator->DeallocateAligned(m_pBuf);
}
//---------------------------------------------------------------------------
void String::clear()
{
	if (m_pBuf != m_default)
		s_pAllocator->DeallocateAligned(m_pBuf);

	m_pBuf = m_default;
	m_pBuf[0] = 0;
}
//---------------------------------------------------------------------------
size_t String::length() const
{
	return strlen(m_pBuf);
}
//---------------------------------------------------------------------------
String::String(const String& other)
{
	m_default[0] = 0;
	m_pBuf = m_default;
	m_len = 0;
	*this = other;
}
//---------------------------------------------------------------------------
String::String(const char* other)
{
	m_default[0] = 0;
	m_pBuf = m_default;
	m_len = 0;
	*this = other;
}
//---------------------------------------------------------------------------
String& String::operator=(const String& other)
{
	size_t otherLen = other.m_len;

	if (otherLen > m_len)
	{
		if ((otherLen + 1) > DEFAULT_BUFLEN)
		{
			*m_pBuf = 0;

			// if we need to expand into the heap storage...
			if (m_pBuf != m_default)
			{
				s_pAllocator->DeallocateAligned(m_pBuf);
			}

			size_t newLen = otherLen + 1;
			// round up to next 4 byte length, but only if needed
			newLen = ((newLen + 3) & ~0x03);
			m_pBuf = s_pAllocator->AllocateAligned(newLen, 8 COS_ALLOC_SITE);
		}
	}

	// other string will fit into our current storage
	m_len = otherLen;
	strcpy_s(m_pBuf, otherLen+1, other.m_pBuf);
	return *this;
}
//---------------------------------------------------------------------------
String& String::operator=(const char* other)
{
	size_t otherLen = strlen(other);

	if (otherLen > m_len)
	{
		if ((otherLen + 1) > DEFAULT_BUFLEN)
		{
			*m_pBuf = 0;

			// if we need to expand into the heap storage...
			// if we need to expand into the heap storage...
			if (m_pBuf != m_default)
			{
				s_pAllocator->DeallocateAligned(m_pBuf);
			}

			size_t newLen = otherLen + 1;
			// round up to next 4 byte length, but only if needed
			newLen = ((newLen + 3) & ~0x03);
			m_pBuf = s_pAllocator->AllocateAligned(newLen, 8 COS_ALLOC_SITE);
		}
	}

	// other string will fit into our current storage
	m_len = otherLen;
	strcpy_s(m_pBuf, otherLen+1, other);
	return *this;
}
//---------------------------------------------------------------------------
String& String::operator+=(const char* other)
{
	size_t newLen = _resize(strlen(other));
	strcat_s(m_pBuf, newLen, other);
	return *this;
}
//---------------------------------------------------------------------------
String& String::operator+=(const String& other)
{
	return (*this += other.m_pBuf);
}
//---------------------------------------------------------------------------
String operator+(const String& l, const String& r)
{
	String rtn(l);
	return (rtn += r);
}
//---------------------------------------------------------------------------
String operator+(const String& l, const char* r)
{
	String rtn(l);
	return (rtn += r);
}
//---------------------------------------------------------------------------
String operator+(const char* l, const String& r)
{
	String rtn(l);
	return (rtn += r);
}
//---------------------------------------------------------------------------
bool String::operator==(const String& other) const
{
	return (0 == strcmp(m_pBuf, other.m_pBuf));
}
//---------------------------------------------------------------------------
bool String::operator==(const char* other) const
{
	return (0 == strcmp(m_pBuf, other));
}
//---------------------------------------------------------------------------
bool String::operator!=(const String& other) const
{
	return !(*this == other);
}
//---------------------------------------------------------------------------
bool String::operator!=(const char* other) const
{
	return !(*this == other);
}
//---------------------------------------------------------------------------
bool String::operator<(const String& other) const
{
	return (strcmp(m_pBuf, other.m_pBuf) < 0);
}
//---------------------------------------------------------------------------
String& String::append(const String& other)
{
	return (*this += other);
}
//---------------------------------------------------------------------------
String& String::append(const char* other)
{
	return (*this += other);
}
//---------------------------------------------------------------------------
size_t String::findFirst(char c) const
{
	size_t rtn = 0;
	const char* p = m_pBuf;

	while (*p)
	{
		if (*p++ == c)
			return rtn;

		++rtn;
	}

	return INVALID;
}
//---------------------------------------------------------------------------
size_t String::findLast(char c) const
{
	size_t rtn = m_len - 1;
	const char* p = m_pBuf;
	p += rtn;

	while (*p)
	{
		if (*p-- == c)
			return rtn;

		--rtn;
	}

	return INVALID;
}
//---------------------------------------------------------------------------
void String::truncate(size_t len)
{
	// fast-and-easy version: put a null in the desired place
	if (len < m_len)
	{
		m_pBuf[len] = 0;
		m_len = len;
	}
}
//---------------------------------------------------------------------------
String String::substr(size_t begin, size_t end) const
{
	if (end >= begin)
	{
		// case 1: end is within the limits of this string
		if (end < m_len)
		{
			String rtn(m_pBuf + begin);
			rtn.truncate(end - begin);
			return rtn;
		}

		// case 2: end is equal to or greater than the length of us (i.e INVALID)
		if (end >= m_len)
		{
			return String(m_pBuf + begin);
		}
	}

	return EMPTY;
}
//---------------------------------------------------------------------------
bool String::contains(const char* other) const
{
	return (0 != strstr(m_pBuf, other));
}
//---------------------------------------------------------------------------
bool String::contains(const String& other) const
{
	return (0 != strstr(m_pBuf, other.m_pBuf));
}
//---------------------------------------------------------------------------
size_t String::_resize(size_t otherLen)
{
	size_t myBuflen = ((m_len + 3) & ~0x03);
	size_t needed = otherLen + m_len;
	size_t newLen = needed + 1;

	if (newLen > myBuflen)
	{
		if (newLen > DEFAULT_BUFLEN)
		{
			// round up to next 4 byte length, but only if needed
			newLen = ((newLen + 3) & ~0x03);
			char* newBuf = (char*)s_pAllocator->AllocateAligned(newLen, 8 COS_ALLOC_SITE);
			strcpy_s(newBuf, newLen, m_pBuf);
			*m_pBuf = 0;

			if (m_pBuf != m_default)
			{
				s_pAllocator->DeallocateAligned(m_pBuf);
			}

			m_pBuf = newBuf;
		}
	}

	m_len = needed;

	return newLen;
}
//---------------------------------------------------------------------------
bool String::serialize(Stream& strm) const
{
	strm.write(&m_len, sizeof(m_len));
	strm.write(m_pBuf, m_len);

	return true;
}
//---------------------------------------------------------------------------
bool String::deserialize(Stream& strm)
{
	// strings are serialized with a 16-bit char count, then the string chars
	unsigned short len;
	strm.read(&len, sizeof(unsigned short));
	_resize(len);
	strm.read(m_pBuf, len);

	return true;
}
//---------------------------------------------------------------------------
char& String::operator [](size_t idx)
{
	if (idx > m_len)
		return m_pBuf[m_len]; // return the null terminator

	return m_pBuf[idx];
}