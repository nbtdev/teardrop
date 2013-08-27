/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TD_STRING_INCLUDED)
#define TD_STRING_INCLUDED

#include "Serialization/SerialPointer.h"
#include "Serialization/Serialization.h"
#include "Memory/Memory.h"

namespace Teardrop
{
	class Allocator;
	class ResourceSerializer;
	class Stream;

	/*
		TD String class, UTF-8 only, serializable
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
		void replaceAll(char find, char with);

		/* NOTE: THESE RETURN COPIES */
		String substr(size_t begin=0, size_t end=INVALID) const;

		/*
			Serialization
		*/
		bool serialize(ResourceSerializer& serializer) const;
		bool serialize(Stream& strm) const;
		bool deserialize(Stream& strm);

		TD_DECLARE_ALLOCATOR();

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

#endif // TD_STRING_INCLUDED
