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
#include "GfxTextureD3D9.h"
#include "GfxCommon.h"
#include "GfxRenderer.h"
#include "GfxRendererD3D9.h"
#include "Util/include/Environment.h"
#include "Serialization/include/ResourceSerializer.h"
#include <assert.h>
#include <memory.h>
#include <new.h>

#include <string>

using namespace CoS;
//---------------------------------------------------------------------------
DEFINE_SERIALIZABLE(GfxTextureD3D9);
//---------------------------------------------------------------------------
static D3DFORMAT s_lut[] = 
{
	D3DFMT_A8R8G8B8,
	D3DFMT_X8R8G8B8,
	D3DFMT_DXT1,
	D3DFMT_DXT2,
	D3DFMT_DXT3,
	D3DFMT_DXT4,
	D3DFMT_DXT5,

	D3DFMT_R32F,
	D3DFMT_G32R32F,
	D3DFMT_A32B32G32R32F,
};
//---------------------------------------------------------------------------
GfxTextureD3D9::GfxTextureD3D9()
: m_pTex(0)
{
}
//---------------------------------------------------------------------------
GfxTextureD3D9::GfxTextureD3D9(int i)
: GfxTexture(i), m_pTex(0)
{
}
//---------------------------------------------------------------------------
GfxTextureD3D9::~GfxTextureD3D9()
{
	assert(!m_pTex);
}
//---------------------------------------------------------------------------
bool GfxTextureD3D9::initialize(
	unsigned int width, 
	unsigned int height, 
	unsigned int depth, 
	Format format, 
	Type type,
	size_t bDynamic,
	unsigned int mips, 
	void* pData,
	unsigned int dataLen)
{
	Environment& env = Environment::get();
	bool baseRtn = GfxTexture::initialize(
		width, 
		height, 
		depth, 
		format, 
		type,
		bDynamic,
		mips, 
		pData,
		dataLen);

	if (env.isOffline)
	{
		return true;
	}

	if (baseRtn)
	{
		return initD3D9Tex(bDynamic);
	}

	return true;
}
//---------------------------------------------------------------------------
bool GfxTextureD3D9::initD3D9Tex(size_t bDynamic)
{
	// make a D3D9 texture resource, in the default pool
	Environment& env = Environment::get();
	IDirect3DTexture9* pTex;
	GfxRendererD3D9* pRend = static_cast<GfxRendererD3D9*>(env.pRenderer);
	IDirect3DDevice9* pDevice = pRend->getDevice();
	HRESULT hr;

	DWORD usage = 0;
	if (m_type & TEX_RENDERTARGET)
		usage = D3DUSAGE_RENDERTARGET;
	else
	{
		if (bDynamic)
		{
			usage = D3DUSAGE_DYNAMIC;
			m_type = Type(m_type | TEX_DYNAMIC);
		}
	}

	// because "0" actually means "generate them all", when what we really 
	// meant was "generate nothing beyond the first one", which would be "1"
	UINT actualMips = m_numMips ? m_numMips : 1;

	// use D3DX to load DDS textures, another way otherwise
	if (m_format < GfxTexture::DXT1 || m_format > GfxTexture::DXT5)
	{
		hr = pDevice->CreateTexture(
			(UINT)m_width, 
			(UINT)m_height,
			actualMips,
			usage,
			s_lut[m_format],
			D3DPOOL_DEFAULT,
			&pTex,
			0);
	}
	else
	{
		hr = D3DXCreateTextureFromFileInMemoryEx(
			pDevice,
			m_pData,
			(UINT)m_dataLen,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			(UINT)m_numMips,
			0,
			s_lut[m_format],
			D3DPOOL_DEFAULT,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			0,
			0,
			0,
			&pTex);
	}

	if (FAILED(hr))
	{
		pRend->logD3DError(hr);
		return false;
	}

	m_pTex = pTex;

	pRend->trackTexture(this, true);
	return true;
}
//---------------------------------------------------------------------------
bool GfxTextureD3D9::delayInit()
{
	return initD3D9Tex((m_type & TEX_DYNAMIC) == TEX_DYNAMIC);
}
//---------------------------------------------------------------------------
bool GfxTextureD3D9::release()
{
	releaseD3DResources();
	GfxRendererD3D9* pRend = static_cast<GfxRendererD3D9*>(
		Environment::get().pRenderer);
	pRend->trackTexture(this, false);
	return true;
}
//---------------------------------------------------------------------------
void GfxTextureD3D9::releaseD3DResources()
{
	if (m_pTex)
	{
		m_pTex->Release();
		m_pTex = 0;
	}
}
//---------------------------------------------------------------------------
bool GfxTextureD3D9::destroy()
{
	return release() && GfxTexture::destroy();
}
//---------------------------------------------------------------------------
void* GfxTextureD3D9::lock(int& pitch, bool /*bDiscardExisting*/)
{
	pitch = 0;
	if (m_pTex)
	{
		D3DLOCKED_RECT lockedRect;
		if (SUCCEEDED(m_pTex->LockRect(
			0,
			&lockedRect,
			0,
			D3DLOCK_DISCARD)))
		{
			pitch = lockedRect.Pitch;
			return lockedRect.pBits;
		}
	}
	else
		return m_pData;

	return 0;
}
//---------------------------------------------------------------------------
void GfxTextureD3D9::unlock()
{
	if (m_pTex)
		m_pTex->UnlockRect(0);
}
//---------------------------------------------------------------------------
IDirect3DTexture9* GfxTextureD3D9::getD3D9Texture()
{
	if (!m_pTex)
	{
		delayInit();
	}

	return m_pTex;
}
//---------------------------------------------------------------------------
bool GfxTextureD3D9::setToSampler(size_t sampler)
{
	// set this texture on the device in sampler 'sampler'
	Environment& env = Environment::get();
	GfxRendererD3D9* pRend = static_cast<GfxRendererD3D9*>(env.pRenderer);
	IDirect3DDevice9* pDevice = pRend->getDevice();

	if (!m_pTex)
	{
		delayInit();
	}

	HRESULT hr = pDevice->SetTexture((DWORD)sampler, m_pTex);
	if (FAILED(hr))
	{
		pRend->logD3DError(hr);
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
bool GfxTextureD3D9::serialize(ResourceSerializer& serializer)
{
	// relocate the vtabl pointer
	serializer.relocate(GfxTextureD3D9::getSerialClass()->getId());

	serializer.relocate(this, sizeof(GfxTextureD3D9));

	// relocate the serial pointer
	serializer.relocate(m_pData);

	// and then relocate the actual data
	size_t sz = getDataSize(); // TODO: get size of data here...
	serializer.relocate(m_pData, sz);

	return true;
}
