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

#if !defined(SERIALPOINTERARRAY_INCLUDED)
#define SERIALPOINTERARRAY_INCLUDED

#include "Serialization/SerialPointer.h"

namespace CoS
{
	struct SerialPointerArrayBase
	{
		__int64 sz;
		SerialPointer< SerialPointer<void> > first;
		void _init() { sz=0; first=0; } // use only if you know what you are doing and why

		COS_DECLARE_ALLOCATOR();
	};

	template<typename T>
	struct SerialPointerArray : SerialPointerArrayBase
	{
		SerialPointerArray() { _init(); }
		~SerialPointerArray() { delete [] first; }
		bool operator!() { return sz == 0; }
		SerialPointerArray<T>& operator=(const SerialPointerArray<T>& rhs) { first=rhs.first; sz=rhs.sz; return *this; }
		SerialPointer<T>& operator[](size_t i) const 
		{ 
			if (i >= sz)
			{
				return (SerialPointer<T>&)first[sz-1];
			}

			return (SerialPointer<T>&)first[i]; 
		}

		size_t size() const { return (size_t)sz; }
		void clear() { delete [] first; _init(); }

		void resize(size_t size)
		{
			SerialPointer< SerialPointer<void> > p(first);
			first = COS_NEW SerialPointer<T>[size];
			size_t i=0;
			for (; i<sz && i<size; ++i)
			{
				first[i] = p[i];
			}

			sz = size;
			delete [] p;
		}

		SerialPointer<T>& push_back(T* t)
		{
			SerialPointer< SerialPointer<void> > p(first);
			first = COS_NEW SerialPointer<T>[(size_t)sz+1];
			size_t i=0;
			for (; i<sz; ++i)
			{
				first[i] = p[i];
			}
			first[i] = t;
			++sz;
			delete [] p;
			return (SerialPointer<T>&)first[sz-1];
		}

		COS_DECLARE_ALLOCATOR();
	};
}

#endif // SERIALPOINTERARRAY_INCLUDED
