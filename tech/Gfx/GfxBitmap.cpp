/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/


#include "GfxBitmap.h"
#include "GfxCommon.h"
#include "Stream/FileStream.h"
#include <string>
#include <assert.h>

using namespace Teardrop;
//---------------------------------------------------------------------------
#pragma pack(push, 1)
struct TGA_HEADER
{
	char  identsize;          // size of ID field that follows 18 byte header (0 usually)
	char  colourmaptype;      // type of colour map 0=none, 1=has palette
	char  imagetype;          // type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed

	short colourmapstart;     // first colour map entry in palette
	short colourmaplength;    // number of colours in palette
	char  colourmapbits;      // number of bits per palette entry 15,16,24,32

	short xstart;             // image x origin
	short ystart;             // image y origin
	short width;              // image width in pixels
	short height;             // image height in pixels
	char  bits;               // image bits per pixel 8,16,24,32
	char  descriptor;         // image descriptor bits (vh flip bits)
    
	// pixel data follows header
    
};
#pragma pack(pop)
//---------------------------------------------------------------------------
GfxBitmap::GfxBitmap()
{
	m_pData = 0;
	m_width = 0;
	m_height = 0;
	m_depth = 0;
	m_mips = 0;
	m_fmt = UNKNOWN;
}
//---------------------------------------------------------------------------
GfxBitmap::~GfxBitmap()
{
	if (m_pData)
	{
		GFX_FREE(m_pData);
	}
}
//---------------------------------------------------------------------------
bool GfxBitmap::loadTGA(const char* pFilename)
{
	assert(!m_pData);
	if (m_pData)
	{
		return false;
	}

	TGA_HEADER hdr;

	FileStream strm;
	if (!strm.open(pFilename, READ))
	{
		return false;
	}

	strm.read(&hdr, sizeof(hdr));

	m_width = hdr.width;
	m_height = hdr.height;
	m_depth = (hdr.bits >> 3);

	size_t dataSize = m_width * m_height * m_depth;
	m_pData = GFX_ALLOCATE(dataSize);
	if (!strm.read(m_pData, dataSize))
	{
		GFX_FREE(m_pData);
		m_pData = 0;
		return false;
	}

	strm.close();
	m_fmt = TGA;
	return true;
}
//---------------------------------------------------------------------------
bool GfxBitmap::loadRawData(Stream& strm)
{
#if defined(_WIN32) || defined(_WIN64)
	size_t dataSize = strm.length();
	m_pData = GFX_ALLOCATE(dataSize);
	if (!strm.read(m_pData, dataSize))
	{
		GFX_FREE(m_pData);
		m_pData = 0;
		return false;
	}

	m_fmt = UNKNOWN;
	// TODO: figure out a better way not to have D3DX in this file...
	D3DXIMAGE_INFO info;
	HRESULT hr = D3DXGetImageInfoFromFileInMemory(
		m_pData, (UINT)dataSize, &info);

	if (FAILED(hr))
	{
		// todo: log this?
		return false;
	}

	switch (info.Format)
	{
	case D3DFMT_A8R8G8B8:
	case D3DFMT_R8G8B8:
		m_fmt = TGA;
		break;

	case D3DFMT_DXT1:
		m_fmt = DXT1;
		break;

	case D3DFMT_DXT2:
		m_fmt = DXT2;
		break;

	case D3DFMT_DXT3:
		m_fmt = DXT3;
		break;

	case D3DFMT_DXT4:
		m_fmt = DXT4;
		break;

	case D3DFMT_DXT5:
		m_fmt = DXT5;
		break;

	default:
		m_fmt = UNKNOWN;
		break;
	}

	m_width = info.Width;
	m_height = info.Height;
	m_depth = info.Depth;
	m_mips = info.MipLevels;

	return true;
#else // _WIN32, _WIN64
    return false;
#endif // _WIN32, _WIN64
}
//---------------------------------------------------------------------------
bool GfxBitmap::loadRawData(const char* pFilename)
{
	// simply open the file, slurp in the contents, and carry on
	FileStream strm;
	if (!strm.open(pFilename, READ))
	{
		return false;
	}

	bool rtn = loadRawData(strm);
	strm.close();

	return rtn;
}
