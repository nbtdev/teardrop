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
#include "GfxMatrixPaletteCache.h"
#include "GfxCommon.h"
#include "Math/Matrix44.h"
#include "Util/Environment.h"
#include <set>
#include <map>
#include <new.h>

using namespace CoS;
//---------------------------------------------------------------------------
typedef std::set<Matrix44*> Palettes;
struct PaletteCache : public std::map<size_t, Palettes>
{
	DECLARE_GFX_ALLOCATOR();
};
struct GfxMatrixPaletteCache::CacheEntry
{
	PaletteCache cache;

	DECLARE_GFX_ALLOCATOR();
};
//---------------------------------------------------------------------------
GfxMatrixPaletteCache::GfxMatrixPaletteCache()
{
	m_pCache = 0;
}
//---------------------------------------------------------------------------
GfxMatrixPaletteCache::~GfxMatrixPaletteCache()
{
}
//---------------------------------------------------------------------------
bool GfxMatrixPaletteCache::initialize()
{
	m_pCache = COS_NEW CacheEntry;
	return true;
}
//---------------------------------------------------------------------------
bool GfxMatrixPaletteCache::destroy()
{
	if (m_pCache)
	{
		clear();

		delete m_pCache;
		m_pCache = 0;
	}

	return true;
}
//---------------------------------------------------------------------------
Matrix44* GfxMatrixPaletteCache::getMatrixPalette(size_t size)
{
	if (!m_pCache)
	{
		return 0;
	}

	PaletteCache::iterator it = m_pCache->cache.find(size);

	// if there is no entry for this size, make one
	if (it == m_pCache->cache.end())
	{
		m_pCache->cache[size] = Palettes();
		it = m_pCache->cache.find(size);
	}

	if(!it->second.size())
	{
		// add a new one
		it->second.insert(
			(Matrix44*)GFX_ALLOCATE_ALIGNED(sizeof(Matrix44)*size, 16));
	}

	Palettes::iterator i = it->second.begin();
	Matrix44* pRtn = *i;
	it->second.erase(i);
	return pRtn;
}
//---------------------------------------------------------------------------
void GfxMatrixPaletteCache::releaseMatrixPalette(
	const Matrix44* pPalette, size_t size)
{
	if (!m_pCache)
	{
		return;
	}

	// in case someone sends us something they didn't get from us originally
	PaletteCache::iterator it = m_pCache->cache.find(size);

	// if there is no entry for this size, make one
	if (it == m_pCache->cache.end())
	{
		m_pCache->cache[size] = Palettes();
		it = m_pCache->cache.find(size);
	}

	it->second.insert(const_cast<Matrix44*>(pPalette));
}
//---------------------------------------------------------------------------
void GfxMatrixPaletteCache::clear()
{
	for (PaletteCache::iterator it = m_pCache->cache.begin();
		it != m_pCache->cache.end(); ++it)
	{
		for (Palettes::iterator it2 = it->second.begin(); 
			it2 != it->second.end(); ++it2)
		{
			GFX_FREE(*it2);
		}
	}

	m_pCache->cache.clear();
}
