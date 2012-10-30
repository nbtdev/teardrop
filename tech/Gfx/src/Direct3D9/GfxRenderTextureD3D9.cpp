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
#include "GfxRenderTextureD3D9.h"
#include "GfxTextureD3D9.h"
#include "GfxRendererD3D9.h"
#include "Gfx/include/GfxRenderConfig.h"
#include "Gfx/include/GfxTexture.h"
#include "Resource/include/ResourceManager.h"
#include "Util/include/Hash.h"
#include "Util/include/Environment.h"

using namespace CoS;
//---------------------------------------------------------------------------
const unsigned __int64 GfxRenderTextureD3D9::IID = 
	hashString64("GfxRenderTextureD3D9");
//---------------------------------------------------------------------------
GfxRenderTextureD3D9::GfxRenderTextureD3D9()
{
	m_hTexture = INVALID_RESOURCE_HANDLE;
	m_pVP = 0;
	m_pSurface = 0;
	m_pDepthStencil = 0;
}
//---------------------------------------------------------------------------
GfxRenderTextureD3D9::~GfxRenderTextureD3D9()
{
	if (m_hTexture != INVALID_RESOURCE_HANDLE)
		Environment::get().pResourceMgr->release(m_hTexture);
}
//---------------------------------------------------------------------------
void* GfxRenderTextureD3D9::getTextureHandle() const
{
	return m_hTexture;
}
//---------------------------------------------------------------------------
IDirect3DSurface9* GfxRenderTextureD3D9::getRenderSurface()
{
	if (!m_pSurface)
	{
		// initialize the texture surface member
		GfxRendererD3D9* pRend = 
			static_cast<GfxRendererD3D9*>(Environment::get().pRenderer);

		GfxTexture* pTex = AutoResPtr<GfxTexture>(m_hTexture);
		if (pTex) 
		{
			GfxTextureD3D9* pD3D9Tex = static_cast<GfxTextureD3D9*>(pTex);
			HRESULT hr = pD3D9Tex->getD3D9Texture()->GetSurfaceLevel(0, &m_pSurface);
			if (FAILED(hr))
			{
				pRend->logD3DError(hr);
				return false;
			}
		}
	}

	return m_pSurface;
}
//---------------------------------------------------------------------------
IDirect3DSurface9* GfxRenderTextureD3D9::getDepthStencil()
{
	return m_pDepthStencil;
}
//---------------------------------------------------------------------------
GfxViewport* GfxRenderTextureD3D9::getFullTargetViewport() const
{
	return m_pVP;
}
//---------------------------------------------------------------------------
void GfxRenderTextureD3D9::onDeviceReset()
{
	if (m_pDepthStencil)
	{
		m_pDepthStencil->Release();
		m_pDepthStencil = 0;
	}

	if (m_pSurface)
	{
		m_pSurface->Release();
		m_pSurface = 0;
	}

	// the underlying texture is handled via a different path, so we don't 
	// need to deal with that here
}
//---------------------------------------------------------------------------
void GfxRenderTextureD3D9::onDeviceRestored()
{
}
//---------------------------------------------------------------------------
static GfxTexture::Format s_texfmt[] = {
	/*
	FMT_A8R8G8B8,
	FMT_R8G8B8,
	FMT_DXT1,
	FMT_DXT2,
	FMT_DXT3,
	FMT_DXT4,
	FMT_DXT5,
	FMT_FLOAT32,
	FMT_FLOAT64,
	FMT_FLOAT128,
	*/

	GfxTexture::TGA,
	GfxTexture::TGA,
	GfxTexture::DXT1,
	GfxTexture::DXT2,
	GfxTexture::DXT3,
	GfxTexture::DXT4,
	GfxTexture::DXT5,
	GfxTexture::TEXF_FLOAT32,
	GfxTexture::TEXF_FLOAT64,
	GfxTexture::TEXF_FLOAT128,
};
//---------------------------------------------------------------------------
bool GfxRenderTextureD3D9::initialize(GfxRenderConfig& cfg, SurfaceFormat fmt)
{
	if (!GfxRenderTarget::initialize(cfg, fmt))
		return false;

	// D3DPOOL_DEFAULT is required for render targets; luckily, all of our
	// textures are created in D3DPOOL_DEFAULT, so just get one from the 
	// resource manager
	m_hTexture = Environment::get().pResourceMgr->acquire(GfxTexture::RESOURCE_TYPE);
	GfxTexture* pTex = AutoResPtr<GfxTexture>(m_hTexture);
	if (pTex)
	{
		size_t depth;
		switch (fmt)
		{
		case FMT_A8R8G8B8:
		case FMT_FLOAT32:
			depth = 4;
			break; 
		case FMT_R8G8B8:
			depth = 3;
			break; 
		default:
			depth = 1;
			break; 
		}

		if (!pTex->initialize(
			cfg.width,
			cfg.height,
			depth,
			s_texfmt[fmt],
			GfxTexture::Type(GfxTexture::TEX2D | GfxTexture::TEX_RENDERTARGET),
			0))
		{
			return false;
		}
	}

	// create full-target viewport
	m_pVP = addViewport(0, 0, 1, 1);

	return true;
}
//---------------------------------------------------------------------------
bool GfxRenderTextureD3D9::destroy()
{
	if (m_pDepthStencil)
	{
		m_pDepthStencil->Release();
		m_pDepthStencil = 0;
	}

	if (m_pSurface)
	{
		m_pSurface->Release();
		m_pSurface = 0;
	}

	if (m_hTexture != INVALID_RESOURCE_HANDLE)
		Environment::get().pResourceMgr->release(m_hTexture);

	m_hTexture = INVALID_RESOURCE_HANDLE;
	return GfxRenderTarget::destroy();
}
//---------------------------------------------------------------------------
bool GfxRenderTextureD3D9::present()
{
#if 0
	HRESULT hr;
	GfxRendererD3D9* pRend = 
		static_cast<GfxRendererD3D9*>(Environment::get().pRenderer);

	if (m_pSwapChain)
	{
		hr = m_pSwapChain->Present(0, 0, 0, 0, 0);
	}
	else
	{
		hr = pRend->getDevice()->Present(0, 0, 0, 0);
	}

	if (FAILED(hr))
		pRend->logD3DError(hr);
#endif
	return true;
}
//---------------------------------------------------------------------------
bool GfxRenderTextureD3D9::queryInterface(unsigned __int64 IID, void **ppI)
{
	if (IID == GfxRenderTarget::IID)
	{
		*ppI = static_cast<GfxRenderTarget*>(this);
		return true;
	}
	else if (IID == GfxRenderTexture::IID)
	{
		*ppI = static_cast<GfxRenderTexture*>(this);
		return true;
	}
	else if (IID == GfxRenderTargetD3D9::IID)
	{
		*ppI = static_cast<GfxRenderTargetD3D9*>(this);
		return true;
	}

	*ppI = 0;
	return false;
}
