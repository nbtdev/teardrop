/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SERIALIZATION_INCLUDED)
#define SERIALIZATION_INCLUDED

#include "Memory/Allocators.h"

namespace Teardrop
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

		TD_DECLARE_ALLOCATOR();
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
