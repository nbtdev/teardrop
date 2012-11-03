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

#include "stdafx.h"
#include "GfxCommon.h"
#include "Memory/Allocators.h"
#include <assert.h>

using namespace CoS;
//---------------------------------------------------------------------------
static Allocator* s_pAllocator = 0;//GetCRTAllocator();
//---------------------------------------------------------------------------
Allocator* CoS::getGfxAllocator()
{
	return s_pAllocator;
}
//---------------------------------------------------------------------------
void CoS::setGfxAllocator(Allocator* pAlloc)
{
	assert(pAlloc && "NULL allocator provided to Gfx system");
	s_pAllocator = pAlloc;
}
//---------------------------------------------------------------------------
void CoS::GfxFree(void* pMem)
{
	s_pAllocator->DeallocateAligned(pMem);
}
#if defined(COS_OPTION_MEMPROFILE)
//---------------------------------------------------------------------------
void* CoS::GfxAllocate(size_t size, const char* file, int line)
{
	return s_pAllocator->AllocateAligned(size, 8, file, line);
}
//---------------------------------------------------------------------------
void* CoS::GfxAllocateAligned(
	size_t size, size_t align, const char* file, int line)
{
	return s_pAllocator->AllocateAligned(size, align, file, line);
}
#else
//---------------------------------------------------------------------------
void* CoS::GfxAllocate(size_t size)
{
	return s_pAllocator->AllocateAligned(size, 8);
}
//---------------------------------------------------------------------------
void* CoS::GfxAllocateAligned(size_t size, size_t align)
{
	return s_pAllocator->AllocateAligned(size, align);
}
#endif //COS_OPTION_MEMPROFILE
