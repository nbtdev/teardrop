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

#if !defined(COSGFXCOMMON_INCLUDED)
#define COSGFXCOMMON_INCLUDED

#include "Config.h"

namespace CoS
{
	//! vertex element semantics -- limited to 8 different semantics
	enum VertexElementSemantic
	{
		UNUSED			= 0,
		POSITION,
		NORMAL,
		COLOR,
		TEXCOORD,
		BINORMAL,
		TANGENT,
		BLENDWEIGHT,
		BLENDINDEX,
	};

	//! vertex element sizes
	enum VertexElementType
	{
		NOTSET = 0,
		FLOAT,
		FLOAT2,
		FLOAT3,
		FLOAT4,
		HALF2,
		HALF4,
		BYTE4,
		VET_COLOR,
	};

	//! renderable primitive type
	enum PrimitiveType
	{
		UNKNOWN = 0,
		POINTLIST,
		LINELIST,
		LINESTRIP,
		TRILIST,
		TRISTRIP,
		TRIFAN,
	};

	enum SurfaceFormat
	{
		FMT_A8R8G8B8,
		FMT_R8G8B8,
		FMT_DXT1,
		FMT_DXT2,
		FMT_DXT3,
		FMT_DXT4,
		FMT_DXT5,
		FMT_FLOAT32,
		FMT_FLOAT64,
		FMT_FLOAT128,
	};

	// generic handle type used by non-Gfx clients to reference Gfx objects
	//const unsigned int INVALID_GFX_HANDLE = 0xFFFFFFFF;
	//typedef unsigned int GfxHandle;

	// put all graphics allocations on 16-byte boundaries
	const size_t DEFAULT_GFX_ALIGNMENT = 16;

	// de-allocator is the same whether profiling or not
	void GfxFree(void*);
	#define GFX_FREE(mem) (GfxFree(mem))
	
	class Allocator;
	Allocator* getGfxAllocator();
	void setGfxAllocator(Allocator* pAlloc);

	// if memory profiling, we need to supply the FILE and LINE args
#if defined(COS_OPTION_MEMPROFILE)
	void* GfxAllocate(size_t size, const char* file, int line);
	void* GfxAllocateAligned(size_t size, size_t align, const char* file, int line);

	#define GFX_ALLOCATE(size) (GfxAllocate(size, __FILE__, __LINE__))
	#define GFX_ALLOCATE_ALIGNED(size, align) (GfxAllocateAligned(size, align, __FILE__, __LINE__))

#define DECLARE_GFX_ALLOCATOR() \
	void* operator new(size_t s, const char* f, int l) { return getGfxAllocator()->Allocate(s, f, l); } \
	void* operator new[](size_t s, const char* f, int l) { return getGfxAllocator()->Allocate(s, f, l); } \
	void operator delete(void* pMem, const char* f, int l) { getGfxAllocator()->Deallocate(pMem); } \
	void operator delete[](void* pMem, const char* f, int l) { getGfxAllocator()->Deallocate(pMem); } \
	void operator delete(void* pMem) { getGfxAllocator()->Deallocate(pMem); } \
	void operator delete[](void* pMem) { getGfxAllocator()->Deallocate(pMem); } \

#define DECLARE_GFX_ALIGNED_ALLOCATOR() \
	void* operator new(size_t s, const char* f, int l) { return getGfxAllocator()->AllocateAligned(s, 16, f, l); } \
	void* operator new[](size_t s, const char* f, int l) { return getGfxAllocator()->AllocateAligned(s, 16, f, l); } \
	void operator delete(void* pMem, const char* f, int l) { getGfxAllocator()->DeallocateAligned(pMem); } \
	void operator delete[](void* pMem, const char* f, int l) { getGfxAllocator()->DeallocateAligned(pMem); } \
	void operator delete(void* pMem) { getGfxAllocator()->DeallocateAligned(pMem); } \
	void operator delete[](void* pMem) { getGfxAllocator()->DeallocateAligned(pMem); } \

#else // COS_OPTION_MEMPROFILE
	void* GfxAllocate(size_t size);
	void* GfxAllocateAligned(size_t size, size_t alignment);

	#define GFX_ALLOCATE(size) (GfxAllocate(size))
	#define GFX_ALLOCATE_ALIGNED(size, align) (GfxAllocateAligned(size, align))

#define DECLARE_GFX_ALLOCATOR() \
	void* operator new(size_t s) { return getGfxAllocator()->Allocate(s); } \
	void* operator new[](size_t s) { return getGfxAllocator()->Allocate(s); } \
	void operator delete(void* pMem) { getGfxAllocator()->Deallocate(pMem); } \
	void operator delete[](void* pMem) { getGfxAllocator()->Deallocate(pMem); } \

#define DECLARE_GFX_ALIGNED_ALLOCATOR() \
	void* operator new(size_t s) { return getGfxAllocator()->AllocateAligned(s, 16); } \
	void* operator new[](size_t s) { return getGfxAllocator()->AllocateAligned(s, 16); } \
	void operator delete(void* pMem) { getGfxAllocator()->DeallocateAligned(pMem); } \
	void operator delete[](void* pMem) { getGfxAllocator()->DeallocateAligned(pMem); } \

#endif // COS_OPTION_MEMPROFILE
}

// since Gfx resource classes have to participate in serialization
#include "Serialization/include/Serialization.h"

// includes SerialPointer.h too
#include "Serialization/include/SerialPointerArray.h"

// base resource handle definition
#include "Resource/include/ResourceHandle.h"

#endif // COSGFXCOMMON_INCLUDED
