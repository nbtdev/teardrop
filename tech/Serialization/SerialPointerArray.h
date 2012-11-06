/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SERIALPOINTERARRAY_INCLUDED)
#define SERIALPOINTERARRAY_INCLUDED

#include "Serialization/SerialPointer.h"

namespace Teardrop
{
	struct SerialPointerArrayBase
	{
		__int64 sz;
		SerialPointer< SerialPointer<void> > first;
		void _init() { sz=0; first=0; } // use only if you know what you are doing and why

		TD_DECLARE_ALLOCATOR();
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
			first = TD_NEW SerialPointer<T>[size];
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
			first = TD_NEW SerialPointer<T>[(size_t)sz+1];
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

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // SERIALPOINTERARRAY_INCLUDED
