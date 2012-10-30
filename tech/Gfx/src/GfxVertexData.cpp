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
#include "GfxVertexData.h"
#include "GfxCommon.h"
#include <assert.h>
#include <new.h>

using namespace CoS;
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
