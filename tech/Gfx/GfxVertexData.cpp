/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "GfxVertexData.h"
#include "GfxCommon.h"
#include <assert.h>
#include <new.h>

using namespace Teardrop;
//---------------------------------------------------------------------------
DEFINE_SERIALIZABLE(GfxVertexData);
//---------------------------------------------------------------------------
GfxVertexData::GfxVertexData()
{
	memset(this, 0, sizeof(GfxVertexData));
}
//---------------------------------------------------------------------------
GfxVertexData::GfxVertexData(int i)
{
	UNREFERENCED_PARAMETER(i);
}
//---------------------------------------------------------------------------
GfxVertexData::~GfxVertexData()
{
}
//---------------------------------------------------------------------------
bool GfxVertexData::initialize(
	CreationFlags flags, 
	unsigned int vertSize,
	unsigned int vertCount, 
	void* pData)
{
	return setVertexData(flags, vertSize, vertCount, pData);
}
//---------------------------------------------------------------------------
bool GfxVertexData::release()
{
	return true;
}
//---------------------------------------------------------------------------
bool GfxVertexData::destroy()
{
	if (m_bDeleteVB)
	{
		GFX_FREE(m_pVertexBuffer);
	}

	m_pVertexBuffer = 0;
	m_vertexCount = 0;
	m_vertexSize = 0;
	m_bDeleteVB = false;

	return true;
}
//---------------------------------------------------------------------------
void* GfxVertexData::lock(bool /*bDiscardExisting*/)
{
	return m_pVertexBuffer;
}
//---------------------------------------------------------------------------
void GfxVertexData::unlock()
{
}
//---------------------------------------------------------------------------
bool GfxVertexData::setVertexData(
	CreationFlags flags, 
	unsigned int vertSize, 
	unsigned int numVerts, 
	void* pData)
{
	m_vertexSize = vertSize;
	m_vertexCount = numVerts;
	m_flags = (unsigned char)flags;

	// if user provided a vertex buffer, use it -- otherwise, make one
	if (pData)
	{
		m_pVertexBuffer = pData;
		m_bDeleteVB = false;
	}
	else
	{
		size_t size = vertSize * numVerts;
		m_pVertexBuffer = GFX_ALLOCATE_ALIGNED(size, 16);
		m_bDeleteVB = true;
	}

	return true;
}
//---------------------------------------------------------------------------
size_t GfxVertexData::getVertexSize() const
{
	return m_vertexSize;
}
//---------------------------------------------------------------------------
void* GfxVertexData::getBuffer()
{
	return m_pVertexBuffer;
}
//---------------------------------------------------------------------------
void GfxVertexData::updateData()
{
	m_bDataChanged = true;
}
//---------------------------------------------------------------------------
size_t GfxVertexData::getVertexCount() const
{
	return m_vertexCount;
}
//---------------------------------------------------------------------------
bool GfxVertexData::serialize(ResourceSerializer& /*serializer*/)
{
	return false;
}
