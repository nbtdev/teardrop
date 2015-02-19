/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(GFXMATRIXPALETTECACHE_INCLUDED)
#define GFXMATRIXPALETTECACHE_INCLUDED

#include <Gfx/GfxCommon.h>
#include <cstddef>

namespace Teardrop
{
	class Matrix44;

	/*
		This class is used by the renderer, as an alternative to constantly
		creating and destroying matrix arrays on the heap. The idea is that 
		when someone is using a matrix of a particular size, the likelihood
		is high that it will use it again in the future. No matrix palettes 
		are created up front -- they are all created on demand, but if a 
		request for one of a particular size can be fulfilled from the ones
		already created and available, then one of those will be used instead.
		Note that the memory managed by this class is grow-only -- it does
		not try to shrink the array or remove unused palette sizes. If you 
		want to release the memory managed by this class, either destroy 
		the instance or call clear().
	*/

	class GfxMatrixPaletteCache
	{
		struct CacheEntry;
		CacheEntry* m_pCache;

	public:
		GfxMatrixPaletteCache();
		~GfxMatrixPaletteCache();

		//! initialize the renderer
		bool initialize();
		//! shutdown the renderer
		bool destroy();
		//! will provide a properly-sized matrix palette, creating one if necessary
		Matrix44* getMatrixPalette(size_t size);
		//! release a palette matrix back to the cache when done with it
		void releaseMatrixPalette(const Matrix44* pPalette, size_t size);
		//! clear all entries that we know about -- make sure that the entries are not used when you call this!
		void clear();

		DECLARE_GFX_ALLOCATOR();

	private:
		// NOT IMPLEMENTED
		GfxMatrixPaletteCache(const GfxMatrixPaletteCache& other);
		GfxMatrixPaletteCache& operator=(const GfxMatrixPaletteCache& other);
	};
}

#endif // GFXMATRIXPALETTECACHE_INCLUDED
