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

#if !defined(SERIALPOINTER_INCLUDED)
#define SERIALPOINTER_INCLUDED

#include "Memory/Allocators.h"

namespace CoS
{
	struct SerialPointerBase
	{
		union u
		{
			unsigned __int64 i64;
			void* pC;
		};
		u _u;

		COS_DECLARE_ALLOCATOR();
	};

	// define 8-byte pointer for all platform word sizes, used for serialization 
	template <typename T>
	struct SerialPointer : SerialPointerBase
	{
		SerialPointer() { _u.i64 = 0; }
		explicit SerialPointer(T* t) { _u.i64 = 0; _u.pC = t; }
		explicit SerialPointer(const SerialPointer& other) { *this = other; }
		template<typename Y> explicit SerialPointer(SerialPointer<Y>& other) { *this = other; }
		SerialPointer& operator=(const SerialPointer& other) { _u = other._u; return *this; }
		template<typename Y> SerialPointer& operator=(SerialPointer<Y>& other) { _u.pC = (T*)other._u.pC; return *this; }
		template<typename Y> SerialPointer& operator=(Y* other) { _u.pC = (T*)other; return *this; }
		//SerialPointer& operator=(size_t i) { _u.i64 = i; return *this; }
		//SerialPointer& operator=(unsigned __int64 i) { _u.i64 = i; return *this; }
		SerialPointer& operator=(T* t) { _u.pC = t; return *this; }
		operator T*() { return (T*)_u.pC; }
		T* operator ->() { return (T*)_u.pC; }
//		operator T() { return *_u.pC; }
		operator const T*() const { return (const T*)_u.pC; }
		bool operator!() const { return _u.pC == 0; }
		bool operator==(T* t) const { return _u.pC == t; }
		bool operator!=(T* t) const { return _u.pC != t; }

		COS_DECLARE_ALLOCATOR();
	};
}

#endif // SERIALPOINTER_INCLUDED
