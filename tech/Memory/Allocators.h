/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(ALLOCATORS_INCLUDED)
#define ALLOCATORS_INCLUDED

#include "Memory/Memory.h"

namespace Teardrop
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

		virtual void* Allocate(size_t size TD_ALLOC_SITE_ARGS) = 0;
		virtual void* AllocateAligned(size_t size, size_t alignment TD_ALLOC_SITE_ARGS) = 0;
		virtual void* Reallocate(void* pMem, size_t newSize TD_ALLOC_SITE_ARGS) = 0;
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

		void* Allocate(size_t size TD_ALLOC_SITE_ARGS);
		void* AllocateAligned(size_t size, size_t alignment TD_ALLOC_SITE_ARGS);
		void* Reallocate(void* pMem, size_t newSize TD_ALLOC_SITE_ARGS);
		void Deallocate(void* pMem);
		void DeallocateAligned(void* pMem);
	};

	// non-region allocator (uses malloc/free)
	class CrtAllocator : public Allocator
	{
	public:
		CrtAllocator(const char* name, size_t softLimit=size_t(-1), size_t hardLimit=size_t(-1), MemoryRegion* pRegion=0); 
		virtual ~CrtAllocator();

		void* Allocate(size_t size TD_ALLOC_SITE_ARGS);
		void* AllocateAligned(size_t size, size_t alignment TD_ALLOC_SITE_ARGS);
		void* Reallocate(void* pMem, size_t newSize TD_ALLOC_SITE_ARGS);
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

		void* Allocate(size_t size TD_ALLOC_SITE_ARGS);
		void* AllocateAligned(size_t size, size_t alignment TD_ALLOC_SITE_ARGS);
		void* Reallocate(void* pMem, size_t newSize TD_ALLOC_SITE_ARGS);
		void Deallocate(void* pMem);
		void DeallocateAligned(void* pMem);
	};
}

#define TD_ALLOCATOR(_name) \
	Teardrop::Allocator* Get##_name##Allocator();

#define TD_ALLOCATOR_IMPL(_name, _allocator, _region, _soft, _hard) \
	Teardrop::Allocator* Get##_name##Allocator() \
	{ static _allocator s_allocator(#_name, _soft, _hard, TD_GET_MEMORY_REGION(_region)); return &s_allocator; }

#define TD_GET_ALLOCATOR(_name) \
	(Get##_name##Allocator())

Teardrop::Allocator* _getAllocatorChain();

TD_ALLOCATOR(DEFAULT)
TD_ALLOCATOR(UNTRACKED)
TD_ALLOCATOR(CRT)

#endif // ALLOCATORS_INCLUDED
