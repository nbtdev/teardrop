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
#include "GfxBitmap.h"
#include "GfxCommon.h"
#include "Stream/include/FileStream.h"
#include <string>
#include <assert.h>

using namespace CoS;
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
bool GfxBitmap::loadBMP(const char* pFilename)
{
	HANDLE hBmp = LoadImage(
		GetModuleHandle(0),
		pFilename,
		IMAGE_BITMAP,
		0, 
		0,
		LR_DEFAULTSIZE | LR_LOADFROMFILE);

	if (hBmp == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	BITMAP bmp;
	GetObject(hBmp, sizeof(BITMAP), &bmp);

	m_fmt = BMP;
	return true;
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
bool GfxBitmap::load(Stream& stream)
{
	return loadRawData(stream);
}
//---------------------------------------------------------------------------
bool GfxBitmap::load(const char* pFilename)
{
	assert(!m_pData);
	if (m_pData)
	{
		return false;
	}

	std::string fname(pFilename);
	std::string ext(fname.substr(fname.find_last_of('.')+1));

	if (ext == "bmp" || ext == "BMP")
	{
		// load Windows bitmap file
		return loadBMP(pFilename);
	}

	if (ext == "tga" || ext == "TGA")
	{
		return loadTGA(pFilename);
	}

	return loadRawData(pFilename);
}
//---------------------------------------------------------------------------
bool GfxBitmap::load(
	void* /*pData*/, Format /*fmt*/, 
	size_t /*sx*/, size_t /*sy*/, size_t /*depth*/)
{
	return true;
}
//---------------------------------------------------------------------------
bool GfxBitmap::loadRawData(Stream& strm)
{
	size_t dataSize = strm.length();
	m_pData = GFX_ALLOCATE(dataSize);
	if (!strm.read(m_pData, dataSize))
	{
		GFX_FREE(m_pData);
		m_pData = 0;
		return false;
	}

	//FourCC* pfcc = (FourCC*)m_pData;

	m_fmt = UNKNOWN;
	//if (*pfcc == DDS)
	//{
	//	m_fmt = DXTC;
	//}
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
