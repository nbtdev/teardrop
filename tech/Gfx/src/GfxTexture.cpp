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
#include "GfxTexture.h"
#include "GfxCommon.h"
#include "GfxBitmap.h"
#include "Util/include/FourCC.h"
#include "Stream/include/Stream.h"
#include <assert.h>
#include <memory.h>
#include <new.h>

#include <string>

using namespace CoS;
//---------------------------------------------------------------------------
DEFINE_SERIALIZABLE(GfxTexture);
const FourCC& GfxTexture::RESOURCE_TYPE = FourCC('T','E','X',' ');
//---------------------------------------------------------------------------
GfxTexture::GfxTexture()
{
	memset(this, 0, sizeof(GfxTexture));
}
//---------------------------------------------------------------------------
GfxTexture::GfxTexture(int i)
{
	UNREFERENCED_PARAMETER(i);
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
bool GfxTexture::loadTextureData(Stream& stream)
{
	// for now, go by extension -- we support .dds and .tga...and *that's all*
	GfxBitmap image;
	size_t len = stream.length();

	if (!image.load(stream))
	{
		return false;
	}

	// otherwise, we need to make a buffer to store the stream's contents
	m_pData = GFX_ALLOCATE(len);
	memcpy(m_pData, image.getData(), len);

	Format fmt = GfxTexture::UNKNOWN;

	switch (image.getFormat())
	{
	case GfxBitmap::DXT1:
		fmt = GfxTexture::DXT1;
		break;

	case GfxBitmap::DXT2:
		fmt = GfxTexture::DXT2;
		break;

	case GfxBitmap::DXT3:
		fmt = GfxTexture::DXT3;
		break;

	case GfxBitmap::DXT4:
		fmt = GfxTexture::DXT4;
		break;

	case GfxBitmap::DXT5:
		fmt = GfxTexture::DXT5;
		break;

	case GfxBitmap::TGA:
		fmt = GfxTexture::TGA;
		break;

	default:
		return false;
		break;
	}

	return initialize(
		(unsigned int)image.getWidth(),
		(unsigned int)image.getHeight(),
		(unsigned int)image.getDepth(),
		fmt,
		GfxTexture::TEX2D,
		0,
		(unsigned int)image.getMips(),
		m_pData,
		(unsigned int)len);
}
//---------------------------------------------------------------------------
bool GfxTexture::setToSampler(size_t /*sampler*/)
{
	// is implemented by derived classes
	return true;
}
//---------------------------------------------------------------------------
bool GfxTexture::serialize(ResourceSerializer& /*serializer*/)
{
	return false;
}
