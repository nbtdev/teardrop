/******************************************************************************
Copyright (c) 2018 Teardrop Games

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
******************************************************************************/

#include "Memory.h"
#include "Allocators.h"
#include "MemoryRegion.h"
#include <assert.h>
#include <stdlib.h>

// thanks MSVC...
#if (defined(_WIN32) || defined(_WIN64)) && !defined(_NOEXCEPT)
    #define _NOEXCEPT noexcept
#else
    #define _NOEXCEPT
#endif // _NOEXCEPT

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
void operator delete(void* pMem) _NOEXCEPT
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
void operator delete[](void* pMem) _NOEXCEPT
{
	// see above
#if !defined(_DEBUG)
	return free(pMem);
#else
	//GetCRTAllocator()->Deallocate(pMem);
	return free(pMem);
#endif
}
