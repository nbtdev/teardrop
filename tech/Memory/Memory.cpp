/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Memory.h"
#include "Allocators.h"
#include "MemoryRegion.h"
#include <assert.h>
#include <stdlib.h>

using namespace Teardrop;
//-----------------------------------------------------------------------------
void* operator new(size_t size)
{
	// commented out until we figure out how to avoid infinite recursion on memprofile
	//const char* pFilename = 0;
	//int line = 0;
	// facet/locale are rogue and always destroy after the app's static objects
	// have been destroyed, so as long as we are using std::stringstream (or
	// any STL class that uses facet/locale to stream something) we can't use
	// any of our own stuff for otherwise-CRT allocations in anything but Debug
	// builds
#if !defined(_DEBUG)
	return malloc(size);
#else
	//return GetCRTAllocator()->Allocate(size TD_ALLOC_SITE_PASSTHRU);
	return malloc(size);
#endif
}
//-----------------------------------------------------------------------------
void* operator new[](size_t size)
{
	// commented out until we figure out how to avoid infinite recursion on memprofile
	//const char* pFilename = 0;
	//int line = 0;
	// see above
#if !defined(_DEBUG)
	return malloc(size);
#else
	//return GetCRTAllocator()->Allocate(size TD_ALLOC_SITE_PASSTHRU);
	return malloc(size);
#endif
}
//-----------------------------------------------------------------------------
void operator delete(void* pMem) noexcept
{
	// see above
#if !defined(_DEBUG)
	return free(pMem);
#else
	//GetCRTAllocator()->Deallocate(pMem);
	return free(pMem);
#endif
}
//-----------------------------------------------------------------------------
void operator delete[](void* pMem) noexcept
{
	// see above
#if !defined(_DEBUG)
	return free(pMem);
#else
	//GetCRTAllocator()->Deallocate(pMem);
	return free(pMem);
#endif
}
