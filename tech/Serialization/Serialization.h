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

#if !defined(SERIALIZATION_INCLUDED)
#define SERIALIZATION_INCLUDED

#include "Memory/Allocators.h"

namespace CoS
{
	class SerialClass
	{
		const char* name;
		void* addr;
		unsigned __int64 id;
		bool bHasVtabl;

	public:
		SerialClass(const char* name, bool hasVtabl=false);
		const char* getName() const;
		const void* getAddr() const;
		unsigned __int64 getId() const;
		bool hasVtabl() const;
		void setAddr(void* pClassInst);

		SerialClass* pNext;
		static SerialClass* findClassById(unsigned __int64 id);
		static SerialClass* findClassByName(const char* name);

		COS_DECLARE_ALLOCATOR();
	};

// make room at the front of PODs and classes for a full 8-byte vtabl pointer
#if defined(WIN32)
#define DECLARE_SERIALIZABLE_VTABLE int _unused_serial_padding;
#else
#define DECLARE_SERIALIZABLE_VTABLE
#endif

#define DECLARE_SERIALIZABLE(c) \
public: \
	static SerialClass* getSerialClass() \
	{ \
		static SerialClass s_class(#c); \
		return &s_class; \
	} \
	static c* getClassInstance(); \
	static struct SerialClassInit \
	{ \
		SerialClassInit() \
		{ \
			getSerialClass()->setAddr( \
				getClassInstance()); \
		} \
	} s_serialClassInit; \
private: \

#define DEFINE_SERIALIZABLE(c) \
	c::SerialClassInit c::s_serialClassInit; \
	c* c::getClassInstance() \
	{ \
		static c _c; \
		return &_c; \
	} \

}

#endif // SERIALIZATION_INCLUDED
