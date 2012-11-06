/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if !defined(TDMEMORYREGION_INCLUDED)
#define TDMEMORYREGION_INCLUDED

namespace Teardrop
{
	class MemoryRegion
	{
	public:
		MemoryRegion(const char* name, size_t size, void* pBase = 0);
		~MemoryRegion();

		void* Allocate(size_t size);
		void* AllocAligned(size_t size, size_t alignment);
		void* Reallocate(void* pMem, size_t newSize);
		void  Deallocate(void* pMem);
		size_t GetChunkSize(void* pMem);
		void Close();

		const char* GetName() { return m_name; }
		size_t GetSize() { return m_size; }

		size_t GetHighWaterMark() { return m_maxBytes; }
		size_t GetMemoryUsedInBytes() { return m_totalBytes; }

	private:
		void* m_pData;
		size_t m_size;
		char m_name[32];
		void* m_pBase;

		//! for allocation statistics
		size_t m_totalBytes;
		size_t m_maxBytes;
	};
}

#endif // TDMEMORYREGION_INCLUDED
