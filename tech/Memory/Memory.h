/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if !defined(MEMORY_INCLUDED)
#define MEMORY_INCLUDED

#include "Config.h"

#if _MSC_VER > 1490
#include <sal.h>
#endif // _MSC_VER

namespace Teardrop
{
	class MemoryRegion;
	class Allocator;
}

// define the memory space allowed for use by the application
// allow us 256MB of memory space (to start with)
#define TD_DEFAULT_HEAPSIZE (1024 * 1024 * 256)

//! use these instead of the global new/delete operators
#if defined(TD_OPTION_MEMPROFILE)
// disable this because it's spam in the build output
#	pragma warning(disable: 4291)
#	define TD_ALLOC_SITE_ARGS , const char* pFilename, int line
#	define TD_ALLOC_SITE_PASSTHRU , pFilename, line
#	define TD_ALLOC_SITE , __FILE__, __LINE__
#	define TD_NEW new(__FILE__, __LINE__)

#define TD_DECLARE_ALLOCATOR() \
	void* operator new(size_t s, const char* f, int l) { return GetDEFAULTAllocator()->Allocate(s, f, l); } \
	void* operator new[](size_t s, const char* f, int l) { return GetDEFAULTAllocator()->Allocate(s, f, l); } \
	void operator delete(void* pMem, const char* f, int l) { \
		f; \
		l; \
		GetDEFAULTAllocator()->Deallocate(pMem); \
	} \
	void operator delete[](void* pMem, const char* f, int l) { \
		f; \
		l; \
		GetDEFAULTAllocator()->Deallocate(pMem); \
	} \
	void operator delete(void* pMem) { GetDEFAULTAllocator()->Deallocate(pMem); } \
	void operator delete[](void* pMem) { GetDEFAULTAllocator()->Deallocate(pMem); } \

#define TD_DECLARE_ALLOCATOR_CUSTOM(allocator) \
	void* operator new(size_t s, const char* f, int l) { return Get##allocator##Allocator()->Allocate(s, f, l); } \
	void operator delete(void* pMem, const char* f, int l) { Get##allocator##Allocator()->Deallocate(pMem); } \
	void operator delete(void* pMem) { Get##allocatorAllocator()->Deallocate(pMem); } \

#define TD_DECLARE_ALLOCATOR_ALIGNED(align) \
	void* operator new(size_t s, const char* f, int l) { return GetDEFAULTAllocator()->AllocateAligned(s, align, f, l); } \
	void operator delete(void* pMem, const char* f, int l) { GetDEFAULTAllocator()->Deallocate(pMem); } \
	void operator delete(void* pMem) { GetDEFAULTAllocator()->Deallocate(pMem); } \

#define TD_DECLARE_ALLOCATOR_ALIGNED_CUSTOM(align, allocator) \
	void* operator new(size_t s, const char* f, int l) { return Get##allocator##Allocator()->AllocateAligned(s, align, f, l); } \
	void operator delete(void* pMem, const char* f, int l) { Get##allocator##Allocator()->Deallocate(pMem); } \
	void operator delete(void* pMem) { Get##allocatorAllocator()->Deallocate(pMem); } \

	// if memory tracking is on, catch any rogue uses of "new", but we are 
	// forced to do it without knowing from where it came (assert?)
	//void* operator new(size_t size);
	//void* operator new[](size_t size);
	//void operator delete(void* pMem);
	//void operator delete[](void* pMem);

#else // TD_OPTION_MEMPROFILE
#	define TD_ALLOC_SITE_ARGS
#	define TD_ALLOC_SITE
#	define TD_ALLOC_SITE_PASSTHRU
#	define TD_NEW new


// we do not need to send args to delete because the de-alloc is keyed on the 
// memory location being freed (so that info would be superfluous)
void* operator new(size_t size);
_Ret_bytecap_(_Size) void* operator new[](size_t _Size);
void operator delete(void* pMem);
void operator delete[](void* pMem);

#define TD_DECLARE_ALLOCATOR() \
	void* operator new(size_t s) { return GetDEFAULTAllocator()->Allocate(s); } \
	void operator delete(void* pMem) { GetDEFAULTAllocator()->Deallocate(pMem); } \

#define TD_DECLARE_ALLOCATOR_CUSTOM(allocator) \
	void* operator new(size_t s) { return Get##allocator##Allocator()->Allocate(s); } \
	void operator delete(void* pMem) { Get##allocator##Allocator()->Deallocate(pMem); } \

#define TD_DECLARE_ALLOCATOR_ALIGNED(align) \
	void* operator new(size_t s) { return GetDEFAULTAllocator()->AllocateAligned(s, align); } \
	void operator delete(void* pMem) { GetDEFAULTAllocator()->Deallocate(pMem); } \

#define TD_DECLARE_ALLOCATOR_ALIGNED_CUSTOM(align, allocator) \
	void* operator new(size_t s) { return Get##allocator##Allocator()->AllocateAligned(s, align); } \
	void operator delete(void* pMem) { Get##allocator##Allocator()->Deallocate(pMem); } \

#endif // TD_OPTION_MEMPROFILE

#define TD_ALLOCATE(_name, _size) TD_GET_ALLOCATOR(_name)->Allocate(_size TD_ALLOC_SITE)
#define TD_ALLOCATE_ALIGNED(_name, _size, _align) TD_GET_ALLOCATOR(_name)->AllocateAligned(_size, _align TD_ALLOC_SITE)
#define TD_REALLOCATE(_name, _mem, _newSize) TD_GET_ALLOCATOR(_name)->Reallocate(_mem, _newSize TD_ALLOC_SITE)
#define TD_DEALLOCATE(_name, _mem) TD_GET_ALLOCATOR(_name)->Deallocate(_mem)

#define TD_DECLARE_MEMORY_REGION(regionName) \
	Teardrop::MemoryRegion* Get##regionName##Region(); struct regionName {};

#define TD_DEFINE_MEMORY_REGION(regionName, base, regionSize) \
	Teardrop::MemoryRegion* Get##regionName##Region() { static Teardrop::MemoryRegion s_region(#regionName, regionSize, base); return &s_region; }
	
#define TD_GET_MEMORY_REGION(regionName) \
	Get##regionName##Region()

TD_DECLARE_MEMORY_REGION(DEFAULT); // define this in the main.cpp for your program

#include "Memory/Allocators.h"

#endif // MEMORY_INCLUDED
