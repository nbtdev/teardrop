/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/


#include "GfxTexture.h"
#include "GfxCommon.h"
#include "GfxBitmap.h"
#include "Stream/Stream.h"
#include <assert.h>
#include <memory.h>
#include <string>

using namespace Teardrop;
//---------------------------------------------------------------------------
GfxTexture::GfxTexture()
{
	memset(this, 0, sizeof(GfxTexture));
}
//---------------------------------------------------------------------------
GfxTexture::GfxTexture(int /*i*/)
{
	m_pData = 0;
	m_dataLen = 0;
}
//---------------------------------------------------------------------------
GfxTexture::~GfxTexture()
{
	assert(!m_pData);
}
//---------------------------------------------------------------------------
bool GfxTexture::initialize(
	unsigned int width, 
	unsigned int height, 
	unsigned int depth, 
	Format format, 
	Type type,
	size_t /*bDynamic*/,
	unsigned int mips, 
	void* pData,
	unsigned int dataLen)
{
	return setTextureData(
		width, 
		height, 
		depth, 
		format, 
		type,
		mips, 
		pData,
		dataLen
		);
}
//---------------------------------------------------------------------------
bool GfxTexture::release()
{
	return true;
}
//---------------------------------------------------------------------------
bool GfxTexture::destroy()
{
	/*
		this member is nonzero only if the texture data is created (or loaded)
		at runtime;	texture data that exists in texture resources loaded from 
		disk will have this member set to 0 during resource creation.
	*/

	if (m_pData)
	{
		GFX_FREE(m_pData);
		m_pData = 0;
	}

	return true;
}
//---------------------------------------------------------------------------
void* GfxTexture::lock(int& pitch, bool /*bDiscardExisting*/)
{
	pitch = m_depth * m_width;
	return m_pData;
}
//---------------------------------------------------------------------------
void GfxTexture::unlock()
{
}
//---------------------------------------------------------------------------
unsigned int GfxTexture::getWidth() const
{
	return m_width;
}
//---------------------------------------------------------------------------
unsigned int GfxTexture::getHeight() const
{
	return m_height;
}
//---------------------------------------------------------------------------
unsigned int GfxTexture::getDepth() const
{
	return m_depth;
}
//---------------------------------------------------------------------------
GfxTexture::Format GfxTexture::getFormat() const
{
	return m_format;
}
//---------------------------------------------------------------------------
unsigned int GfxTexture::getNumMips() const
{
	return m_numMips;
}
//---------------------------------------------------------------------------
unsigned int GfxTexture::getDataSize() const
{
	return m_dataLen;
}
//---------------------------------------------------------------------------
const char* GfxTexture::getName() const
{
	return 0;
}
//---------------------------------------------------------------------------
GfxTexture::Type GfxTexture::getType() const
{
	return m_type;
}
//---------------------------------------------------------------------------
void GfxTexture::setName(const char* /*pName*/)
{
}
//---------------------------------------------------------------------------
bool GfxTexture::setTextureData(
	unsigned int width, 
	unsigned int height, 
	unsigned int depth, 
	Format format, 
	Type type,
	unsigned int mips, 
	void* pData,
	unsigned int dataLen)
{
	m_width = width;
	m_height = height;
	m_depth = depth;
	m_format = format;
	m_type = type;
	m_numMips = mips;
	m_dataLen = dataLen;

	if (!pData)
	{
		m_dataLen = width * height * depth;
		m_pData = GFX_ALLOCATE(m_dataLen);
	}
	else
	{
		m_pData = pData;
	}

	return true;
}
//---------------------------------------------------------------------------
bool GfxTexture::setToSampler(size_t /*sampler*/)
{
	// is implemented by derived classes
	return true;
}
