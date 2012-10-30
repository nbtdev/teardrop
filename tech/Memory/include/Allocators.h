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

#if !defined(ALLOCATORS_INCLUDED)
#define ALLOCATORS_INCLUDED

#include "Memory/include/Memory.h"

namespace CoS
{
	const size_t		DEFAULT_MEM_ALIGNMENT	= 8;
	const size_t		MAX_ALLOCATOR_NAME_LEN	= 32;
	
	class MemoryRegion;

	struct AllocatorStats
	{
		size_t current;
		size_t highWater;
		size_t hardLimit;
		size_t softLimit;
	};

	// Base allocator class
	class Allocator
	{
	public:
		Allocator(const char* name, size_t softLimit=size_t(-1), size_t hardLimit=size_t(-1), MemoryRegion* pRegion=0);
		virtual ~Allocator();

		virtual void* Allocate(size_t size COS_ALLOC_SITE_ARGS) = 0;
		virtual void* AllocateAligned(size_t size, size_t alignment COS_ALLOC_SITE_ARGS) = 0;
		virtual void* Reallocate(void* pMem, size_t newSize COS_ALLOC_SITE_ARGS) = 0;
		virtual void Deallocate(void* pMem) = 0;
		virtual void DeallocateAligned(void* pMem) = 0;
		virtual void SetLimits(size_t soft, size_t hard) { m_softLimit = soft; m_hardLimit = hard; }

		const char* GetName() { return m_allocatorName; }
		const MemoryRegion* GetMemoryRegion() { return m_pMemRegion; }
		const AllocatorStats& GetStats() { return m_stats; }

		Allocator*		m_pNext;

	protected:
		AllocatorStats	m_stats;
		char			m_allocatorName[MAX_ALLOCATOR_NAME_LEN];
		MemoryRegion*	m_pMemRegion;
		size_t			m_softLimit;
		size_t			m_hardLimit;
	};

	// general-purpose non-thread-safe allocator
	class DefaultAllocator : public Allocator
	{
	public:
		DefaultAllocator(const char* name, size_t softLimit=size_t(-1), size_t hardLimit=size_t(-1), MemoryRegion* pRegion=0) 
			: Allocator(name, softLimit, hardLimit, pRegion) {}
		virtual ~DefaultAllocator() {}

		void* Allocate(size_t size COS_ALLOC_SITE_ARGS);
		void* AllocateAligned(size_t size, size_t alignment COS_ALLOC_SITE_ARGS);
		void* Reallocate(void* pMem, size_t newSize COS_ALLOC_SITE_ARGS);
		void Deallocate(void* pMem);
		void DeallocateAligned(void* pMem);
	};

	// non-region allocator (uses malloc/free)
	class CrtAllocator : public Allocator
	{
	public:
		CrtAllocator(const char* name, size_t softLimit=size_t(-1), size_t hardLimit=size_t(-1), MemoryRegion* pRegion=0); 
		virtual ~CrtAllocator();

		void* Allocate(size_t size COS_ALLOC_SITE_ARGS);
		void* AllocateAligned(size_t size, size_t alignment COS_ALLOC_SITE_ARGS);
		void* Reallocate(void* pMem, size_t newSize COS_ALLOC_SITE_ARGS);
		void Deallocate(void* pMem);
		void DeallocateAligned(void* pMem);
	};

	// non-tracked allocator
	class UntrackedAllocator : public Allocator
	{
	public:
		UntrackedAllocator(const char* name, size_t softLimit=size_t(-1), size_t hardLimit=size_t(-1), MemoryRegion* pRegion=0) 
			: Allocator(name, softLimit, hardLimit, pRegion) {}
		virtual ~UntrackedAllocator() {}

		void* Allocate(size_t size COS_ALLOC_SITE_ARGS);
		void* AllocateAligned(size_t size, size_t alignment COS_ALLOC_SITE_ARGS);
		void* Reallocate(void* pMem, size_t newSize COS_ALLOC_SITE_ARGS);
		void Deallocate(void* pMem);
		void DeallocateAligned(void* pMem);
	};
}

#define COS_ALLOCATOR(_name) \
	CoS::Allocator* Get##_name##Allocator();

#define COS_ALLOCATOR_IMPL(_name, _allocator, _region, _soft, _hard) \
	CoS::Allocator* Get##_name##Allocator() \
	{ static _allocator s_allocator(#_name, _soft, _hard, COS_GET_MEMORY_REGION(_region)); return &s_allocator; }

#define COS_GET_ALLOCATOR(_name) \
	(Get##_name##Allocator())

CoS::Allocator* _getAllocatorChain();

COS_ALLOCATOR(DEFAULT)
COS_ALLOCATOR(UNTRACKED)
COS_ALLOCATOR(CRT)

#endif // ALLOCATORS_INCLUDED
