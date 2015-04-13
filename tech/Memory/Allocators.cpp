/******************************************************************************
Copyright (c) 2015 Teardrop Games

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

#include "Config.h"
#include "Memory.h"
#include "Allocators.h"
#include "MemoryRegion.h"
#include "AllocationTracker.h"
#include "Math/MathUtil.h"
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <new>

namespace {
#if defined(_WIN32) || defined(_WIN64)
    #define STRCPY(d, n, s) strcpy_s(d, n, s)

    void* alloc_aligned(size_t sz, size_t alignment) {
        return _aligned_malloc(sz, alignment);
    }

    void free_aligned(void* p) {
        _aligned_free(p);
    }
#else // _WIN32, _WIN64
    #define STRCPY(d, n, s) strncpy(d, s, n)

    void* alloc_aligned(size_t sz, size_t alignment) {
        void* p = nullptr;
        int rtn = posix_memalign(&p, alignment, sz);

        // TOOD: assert? throw exception? EINVAL means not power of two
        if (rtn) {
            assert(rtn != EINVAL && "Alignment must be a power of two");
            return nullptr;
        }

        return p;
    }

    void free_aligned(void* p) {
        free(p);
    }
#endif // _WIN32, _WIN64

} // namespace

using namespace Teardrop;
//-----------------------------------------------------------------------------
static Allocator* s_pHead = 0;
Allocator* _getAllocatorChain() { return s_pHead; }

#define ALLOCATE_FROM_REGION
//---------------------------------------------------------------------------
static void appendAllocatorDef(Allocator** pRoot, Allocator* pAllocatorDefToAdd)
{
	if (!*pRoot)
	{
		*pRoot = pAllocatorDefToAdd;
		return;
	}

	// else walk the list to find the end, and append there
	Allocator* pTmp = *pRoot;
	while (pTmp->m_pNext)
	{
		pTmp = pTmp->m_pNext;
	}

	pTmp->m_pNext = pAllocatorDefToAdd;
}
//-----------------------------------------------------------------------------
Allocator::Allocator(
	const char* name, 
	size_t hardLimit, 
	size_t softLimit, 
	MemoryRegion* pRegion)
{
    STRCPY(m_allocatorName, MAX_ALLOCATOR_NAME_LEN, name);
	m_pMemRegion = pRegion;
	m_softLimit = softLimit;
	m_hardLimit = hardLimit;
	m_stats.current = 0;
	m_stats.highWater = 0;
	m_stats.hardLimit = hardLimit;
	m_stats.softLimit = softLimit;
	appendAllocatorDef(&s_pHead, this);
}
//-----------------------------------------------------------------------------
Allocator::~Allocator()
{
}
//-----------------------------------------------------------------------------
// Default allocator -- non-thread-safe, use for TLS or for single-threaded
void* DefaultAllocator::Allocate(size_t size TD_ALLOC_SITE_ARGS)
{
#if defined(ALLOCATE_FROM_REGION)
	void* pMem = m_pMemRegion->Allocate(size);
#else
	void* pMem = malloc(size);
#endif
	TD_TRACK_ALLOCATION(pMem, size, GetName());
	size_t sz = m_pMemRegion->GetChunkSize(pMem);
	m_stats.current += sz;
	m_stats.highWater = MathUtil::max(m_stats.highWater, m_stats.current);
	return pMem;
}
//-----------------------------------------------------------------------------
void* DefaultAllocator::AllocateAligned(size_t size, size_t alignment TD_ALLOC_SITE_ARGS)
{
#if defined(ALLOCATE_FROM_REGION)
	void* pMem = m_pMemRegion->AllocAligned(size, alignment);
#else
	void* pMem = _aligned_malloc(size, alignment);
#endif
	TD_TRACK_ALLOCATION(pMem, size, GetName());
	size_t sz = m_pMemRegion->GetChunkSize(pMem);
	m_stats.current += sz;
	m_stats.highWater = MathUtil::max(m_stats.highWater, m_stats.current);
	return pMem;
}
//-----------------------------------------------------------------------------
void* DefaultAllocator::Reallocate(void *pMem, size_t newSize TD_ALLOC_SITE_ARGS)
{
	TD_TRACK_DEALLOCATION(pMem);
	if (pMem)
	{
		size_t sz = m_pMemRegion->GetChunkSize(pMem);
		m_stats.current -= sz;
	}
#if defined(ALLOCATE_FROM_REGION)
	void* pRtn = m_pMemRegion->Reallocate(pMem, newSize);
#else
	void* pRtn = realloc(pMem, newSize);
#endif
	TD_TRACK_ALLOCATION(pMem, newSize, GetName());
	size_t sz = m_pMemRegion->GetChunkSize(pRtn);
	m_stats.current += sz;
	m_stats.highWater = MathUtil::max(m_stats.highWater, m_stats.current);
	return pRtn;
}
//-----------------------------------------------------------------------------
void DefaultAllocator::Deallocate(void *pMem)
{
	if (pMem)
	{
		size_t sz = m_pMemRegion->GetChunkSize(pMem);
		m_stats.current -= sz;
	}
	TD_TRACK_DEALLOCATION(pMem);

#if defined(ALLOCATE_FROM_REGION)
	m_pMemRegion->Deallocate(pMem);
#else
	free(pMem);
#endif
}
//-----------------------------------------------------------------------------
void DefaultAllocator::DeallocateAligned(void *pMem)
{
	if (pMem)
	{
		size_t sz = m_pMemRegion->GetChunkSize(pMem);
		m_stats.current -= sz;
	}
	TD_TRACK_DEALLOCATION(pMem);

#if defined(ALLOCATE_FROM_REGION)
	m_pMemRegion->Deallocate(pMem);
#else
	_aligned_free(pMem);
#endif
}
//-----------------------------------------------------------------------------
// CRT allocator (uses CRT heap for non-controlled allocations, C++ lib, etc)
// Debug CRT heap bookkeeping info from 
// http://www.nobugs.org/developer/win32/debug_crt_heap.html
CrtAllocator::CrtAllocator(
	const char* name, 
	size_t /*softLimit*/, 
	size_t /*hardLimit*/, 
	MemoryRegion* /*pRegion*/)
	: Allocator(name)
{
}
//-----------------------------------------------------------------------------
CrtAllocator::~CrtAllocator()
{
}
//-----------------------------------------------------------------------------
void* CrtAllocator::Allocate(size_t size TD_ALLOC_SITE_ARGS)
{
	void* pMem = malloc(size);
#if defined(_DEBUG)
	size_t* pSz = (size_t*)((char*)pMem - 16);
	m_stats.current += *pSz;
	m_stats.highWater = MathUtil::max(m_stats.highWater, m_stats.current);
#endif
	//TD_TRACK_ALLOCATION(pMem, size, GetName().c_str());
	return pMem;
}
//-----------------------------------------------------------------------------
void* CrtAllocator::AllocateAligned(size_t size, 
	size_t alignment TD_ALLOC_SITE_ARGS)
{
    void* pMem = alloc_aligned(size, alignment);
#if defined(_DEBUG)
	size_t* pSz = (size_t*)((char*)pMem - 16);
	m_stats.current += *pSz;
	m_stats.highWater = MathUtil::max(m_stats.highWater, m_stats.current);
#endif
	//TD_TRACK_ALLOCATION(pMem, size, GetName().c_str());
	return pMem;
}
//-----------------------------------------------------------------------------
void* CrtAllocator::Reallocate(void *pMem, size_t sz TD_ALLOC_SITE_ARGS)
{
#if defined(_DEBUG)
	size_t* pSz = 0;
	if (pMem)
	{
		pSz = (size_t*)((char*)pMem - 16);
		m_stats.current -= *pSz;
	}
#endif

	void* p = realloc(pMem, sz);

#if defined(_DEBUG)
	pSz = (size_t*)((char*)p - 16);
	m_stats.current += *pSz;
	m_stats.highWater = MathUtil::max(m_stats.highWater, m_stats.current);
#endif
	//TD_TRACK_DEALLOCATION(pMem);
	return p;
}
//-----------------------------------------------------------------------------
void CrtAllocator::Deallocate(void *pMem)
{
#if defined(_DEBUG)
	if (pMem)
	{
		size_t* pSz = (size_t*)((char*)pMem - 16);
		m_stats.current -= *pSz;
	}
#endif
	free(pMem);
	//TD_TRACK_DEALLOCATION(pMem);
}
//-----------------------------------------------------------------------------
void CrtAllocator::DeallocateAligned(void *pMem)
{
#if defined(_DEBUG)
	if (pMem)
	{
		size_t* pSz = (size_t*)((char*)pMem - 16);
		m_stats.current -= *pSz;
	}
#endif
    free_aligned(pMem);
	//TD_TRACK_DEALLOCATION(pMem);
}
//-----------------------------------------------------------------------------
// allocator for non-trackable allocations (system, or STL in static init)
void* UntrackedAllocator::Allocate(size_t size TD_ALLOC_SITE_ARGS)
{
#if defined(ALLOCATE_FROM_REGION)
	return m_pMemRegion->Allocate(size);
#else
	return malloc(size);
#endif
}
//-----------------------------------------------------------------------------
void* UntrackedAllocator::AllocateAligned(size_t size, 
	size_t alignment TD_ALLOC_SITE_ARGS)
{
#if defined(ALLOCATE_FROM_REGION)
	return m_pMemRegion->AllocAligned(size, alignment);
#else
	return _aligned_malloc(size, alignment);
#endif
}
//-----------------------------------------------------------------------------
void* UntrackedAllocator::Reallocate(void *pMem, size_t newSize TD_ALLOC_SITE_ARGS)
{
#if defined(ALLOCATE_FROM_REGION)
	return m_pMemRegion->Reallocate(pMem, newSize);
#else
	return realloc(pMem, newSize);
#endif
}
//-----------------------------------------------------------------------------
void UntrackedAllocator::Deallocate(void *pMem)
{
#if defined(ALLOCATE_FROM_REGION)
	m_pMemRegion->Deallocate(pMem);
#else
	free(pMem);
#endif
}
//-----------------------------------------------------------------------------
void UntrackedAllocator::DeallocateAligned(void *pMem)
{
#if defined(ALLOCATE_FROM_REGION)
	m_pMemRegion->Deallocate(pMem);
#else
	_aligned_free(pMem);
#endif
}
//-----------------------------------------------------------------------------
//TD_ALLOCATOR_IMPL(DEFAULT, DefaultAllocator, DEFAULT, 384 * 1024 * 1024, 512 * 1024 * 1024)
//TD_ALLOCATOR_IMPL(UNTRACKED, UntrackedAllocator, DEFAULT, -1, -1)
//TD_ALLOCATOR_IMPL(CRT, CrtAllocator, DEFAULT, -1, -1)
