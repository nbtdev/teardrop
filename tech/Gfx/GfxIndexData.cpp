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
#include "GfxIndexData.h"
#include "GfxCommon.h"
#include <assert.h>
#include <new.h>

using namespace CoS;
//---------------------------------------------------------------------------
DEFINE_SERIALIZABLE(GfxIndexData);
//---------------------------------------------------------------------------
GfxIndexData::GfxIndexData()
{
	memset(this, 0, sizeof(GfxIndexData));
}
//---------------------------------------------------------------------------
GfxIndexData::GfxIndexData(int i)
{
	UNREFERENCED_PARAMETER(i);
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
//---------------------------------------------------------------------------
bool GfxIndexData::serialize(ResourceSerializer& /*serializer*/)
{
	return false;
}
