/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SERIALPOINTER_INCLUDED)
#define SERIALPOINTER_INCLUDED

#include "Memory/Allocators.h"

namespace Teardrop
{
	struct SerialPointerBase
	{
		union u
		{
			unsigned __int64 i64;
			void* pC;
		};
		u _u;

		TD_DECLARE_ALLOCATOR();
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

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // SERIALPOINTER_INCLUDED
