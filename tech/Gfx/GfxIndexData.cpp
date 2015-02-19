/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/


#include "GfxIndexData.h"
#include "GfxCommon.h"
#include <assert.h>
#include <cstring>

using namespace Teardrop;
//---------------------------------------------------------------------------
GfxIndexData::GfxIndexData()
{
	memset(this, 0, sizeof(GfxIndexData));
}
//---------------------------------------------------------------------------
GfxIndexData::GfxIndexData(int /*i*/)
{
}
//---------------------------------------------------------------------------
GfxIndexData::~GfxIndexData()
{
	// if you trip this assert, it means you did not destroy() this object
	assert(!m_pIndexBuffer);
}
//---------------------------------------------------------------------------
bool GfxIndexData::initialize(
	unsigned int indexSize, // in bytes
	unsigned int indexCount, 
	void* pData)
{
	return setIndexData(indexSize, indexCount, pData);
}
//---------------------------------------------------------------------------
bool GfxIndexData::release()
{
	return true;
}
//---------------------------------------------------------------------------
bool GfxIndexData::destroy()
{
	if (m_bDeleteIB)
	{
		GFX_FREE(m_pIndexBuffer);
	}

	m_pIndexBuffer = 0;
	m_indexCount = 0;
	m_indexSize = 0;
	m_bDeleteIB = false;

	return true;
}
//---------------------------------------------------------------------------
void* GfxIndexData::lock(bool /*bDiscardExisting*/)
{
	return m_pIndexBuffer;
}
//---------------------------------------------------------------------------
void GfxIndexData::unlock()
{
}
//---------------------------------------------------------------------------
bool GfxIndexData::setIndexData(
	unsigned int indexSize,
	unsigned int indexCount, 
	void* pData)
{
	// if the user passed in a buffer to use, use it -- otherwise, make a new 
	// one using the params provided
	m_indexSize = indexSize;
	m_indexCount = indexCount;

	if (pData)
	{
		m_pIndexBuffer = pData;
		m_bDeleteIB = false;
	}
	else
	{
		size_t bufSize = indexCount * indexSize;
		m_pIndexBuffer = GFX_ALLOCATE_ALIGNED(bufSize, 16);
		m_bDeleteIB = true;
	}

	return true;
}
//---------------------------------------------------------------------------
void* GfxIndexData::getBuffer()
{
	return m_pIndexBuffer;
}
//---------------------------------------------------------------------------
size_t GfxIndexData::getIndexCount() const
{
	return m_indexCount;
}
//---------------------------------------------------------------------------
size_t GfxIndexData::getIndexSize() const
{
	return m_indexSize;
}
