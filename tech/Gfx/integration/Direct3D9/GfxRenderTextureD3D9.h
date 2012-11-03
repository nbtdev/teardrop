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

#if !defined(GFXRENDERTEXTURED3D9_INCLUDED)
#define GFXRENDERTEXTURED3D9_INCLUDED

#include "GfxRenderTexture.h"
#include "GfxRenderTarget.h"
#include "GfxRenderTargetD3D9.h"

namespace CoS
{
	class GfxRenderTextureD3D9
		: public GfxRenderTexture,	// this is a pure-virtual interface
		public GfxRenderTarget,		// this is the only one with a concrete implementation
		public GfxRenderTargetD3D9	// also a pure-virtual interface
	{
		HResource m_hTexture;
		GfxViewport* m_pVP;
		IDirect3DSurface9* m_pSurface;
		IDirect3DSurface9* m_pDepthStencil;

	public:
		static const unsigned __int64 IID;

		GfxRenderTextureD3D9();
		~GfxRenderTextureD3D9();

		// GfxRenderTarget implementation
		bool initialize(GfxRenderConfig& cfg, SurfaceFormat fmt);
		bool destroy();
		bool queryInterface(unsigned __int64 IID, void** ppInterface);

		// GfxRenderTexture implementation
		void* getTextureHandle() const;
		GfxViewport* getFullTargetViewport() const;

		//! GfxRenderTargetD3D9 implementation
		IDirect3DSurface9* getRenderSurface();
		IDirect3DSurface9* getDepthStencil();
		void onDeviceReset();
		void onDeviceRestored();
		bool present();

		DECLARE_GFX_ALLOCATOR();
	};
}

#endif // GFXRENDERTEXTURED3D9_INCLUDED
