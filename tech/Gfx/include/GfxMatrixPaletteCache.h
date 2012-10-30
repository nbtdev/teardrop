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

#if !defined(GFXMATRIXPALETTECACHE_INCLUDED)
#define GFXMATRIXPALETTECACHE_INCLUDED

namespace CoS
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
