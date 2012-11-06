/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "GfxCommon.h"
#include "Memory/Allocators.h"
#include <assert.h>

using namespace Teardrop;
//---------------------------------------------------------------------------
static Allocator* s_pAllocator = 0;//GetCRTAllocator();
//---------------------------------------------------------------------------
Allocator* Teardrop::getGfxAllocator()
{
	return s_pAllocator;
}
//---------------------------------------------------------------------------
void Teardrop::setGfxAllocator(Allocator* pAlloc)
{
	assert(pAlloc && "NULL allocator provided to Gfx system");
	s_pAllocator = pAlloc;
}
//---------------------------------------------------------------------------
void Teardrop::GfxFree(void* pMem)
{
	s_pAllocator->DeallocateAligned(pMem);
}
#if defined(TD_OPTION_MEMPROFILE)
//---------------------------------------------------------------------------
void* Teardrop::GfxAllocate(size_t size, const char* file, int line)
{
	return s_pAllocator->AllocateAligned(size, 8, file, line);
}
//---------------------------------------------------------------------------
void* Teardrop::GfxAllocateAligned(
	size_t size, size_t align, const char* file, int line)
{
	return s_pAllocator->AllocateAligned(size, align, file, line);
}
#else
//---------------------------------------------------------------------------
void* Teardrop::GfxAllocate(size_t size)
{
	return s_pAllocator->AllocateAligned(size, 8);
}
//---------------------------------------------------------------------------
void* Teardrop::GfxAllocateAligned(size_t size, size_t align)
{
	return s_pAllocator->AllocateAligned(size, align);
}
#endif //TD_OPTION_MEMPROFILE
