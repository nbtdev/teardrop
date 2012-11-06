/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "GfxMatrixPaletteCache.h"
#include "GfxCommon.h"
#include "Math/Matrix44.h"
#include "Util/Environment.h"
#include <set>
#include <map>
#include <new.h>

using namespace Teardrop;
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
	m_pCache = TD_NEW CacheEntry;
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
