/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Memory.h"
#include "MemoryRegion.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <assert.h>

#if defined(max)
#undef max
#endif
#if defined(min)
#undef min
#endif

#include <algorithm>

#define USE_DL_PREFIX
#define MSPACES 1
#pragma warning(push)
#pragma warning(disable: 4267 4312)
#include "malloc-280.h"
#pragma warning(pop)

using namespace Teardrop;
//-----------------------------------------------------------------------------
MemoryRegion::MemoryRegion(const char* name, size_t size, void* pBase)
{
	m_totalBytes = 0;
	m_maxBytes = 0;
	strcpy_s(m_name, 32, name);
	m_pData = 0;
	m_size = size;
	m_pBase = pBase;

	if (!pBase)
	{
		m_pBase = pBase = malloc(size);
	}

	// allow the caller to tell us to use the CRT heap instead, if they like;
	// if m_size is 0, we don't create an mspace and therefore alloc from CRT
	if (m_size)
	{
		m_pData = create_mspace_with_base(pBase, size, 0);
	}
}
//-----------------------------------------------------------------------------
MemoryRegion::~MemoryRegion()
{
	Close();
}
//-----------------------------------------------------------------------------
void MemoryRegion::Close()
{
	if (m_pData)
	{
		destroy_mspace(m_pData);
		m_pData = 0;

		if (m_pBase)
		{
			free(m_pBase);
			m_pBase = 0;
		}
	}
}
//-----------------------------------------------------------------------------
size_t MemoryRegion::GetChunkSize(void* pMem)
{
	if (pMem)
	{
		size_t* pHead = (size_t*)((char*)pMem - sizeof(size_t));
		return (*pHead & size_t(~0x3));
	}

	return 0;
}
//-----------------------------------------------------------------------------
void* MemoryRegion::Allocate(size_t size)
{
	void* pMem = 0;
	if (m_pData)
	{
		pMem = mspace_malloc(m_pData, size);
	}
	else
	{
		pMem = malloc(size);
	}

	if (pMem)
	{
		m_totalBytes += GetChunkSize(pMem);
		m_maxBytes = std::max(m_totalBytes, m_maxBytes);
	}

	return pMem;
}
//-----------------------------------------------------------------------------
void* MemoryRegion::AllocAligned(size_t size, size_t alignment)
{
	void* pMem = 0;
	if (m_pData)
	{
		pMem = mspace_memalign(m_pData, alignment, size);
	}
	else
	{
		pMem = malloc(size);
	}

	if (pMem)
	{
		m_totalBytes += GetChunkSize(pMem);
		m_maxBytes = std::max(m_totalBytes, m_maxBytes);
	}

	return pMem;
}
//-----------------------------------------------------------------------------
void MemoryRegion::Deallocate(void *pMem)
{
	// size of this block is stored in the UInt32 immediately preceding block
	if (pMem)
	{
		m_totalBytes -= GetChunkSize(pMem);
	}

	if (m_pData)
	{
		mspace_free(m_pData, pMem);
	}
}
//-----------------------------------------------------------------------------
void* MemoryRegion::Reallocate(void *pMem, size_t newSize)
{
	// size of this block is stored in the UInt32 immediately preceding block
	if (pMem)
	{
		m_totalBytes -= GetChunkSize(pMem);
	}

	if (m_pData)
	{
		pMem = mspace_realloc(m_pData, pMem, newSize);
	}
	else
	{
		pMem = realloc(pMem, newSize);
	}

	if (pMem)
	{
		m_totalBytes += GetChunkSize(pMem);
		m_maxBytes = std::max(m_totalBytes, m_maxBytes);
	}

	return pMem;
}
