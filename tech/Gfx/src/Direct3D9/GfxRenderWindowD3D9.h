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

#if !defined(GFXRENDERWINDOWD3D9_INCLUDED)
#define GFXRENDERWINDOWD3D9_INCLUDED

#include "GfxRenderWindow.h"
#include "GfxRenderTarget.h"
#include "GfxRenderTargetD3D9.h"

struct IDirect3DSwapChain9;

namespace CoS
{
	class GfxRenderWindowD3D9 
		: public GfxRenderWindow,	// this is a pure-virtual interface
		public GfxRenderTarget,		// this is the only one with a concrete implementation
		public GfxRenderTargetD3D9	// also a pure-virtual interface
	{
		IDirect3DSurface9* m_pSurface;
		IDirect3DSurface9* m_pDepthStencil;
		IDirect3DSwapChain9* m_pSwapChain;
		D3DPRESENT_PARAMETERS m_pparams;
		HWND m_hWnd;

	public:
		GfxRenderWindowD3D9();
		~GfxRenderWindowD3D9();

		bool initialize(
			GfxRenderConfig& cfg,
			SurfaceFormat fmt = FMT_R8G8B8);
		bool destroy();

		//! GfxRenderTargetD3D9 implementation
		IDirect3DSurface9* getRenderSurface();
		IDirect3DSurface9* getDepthStencil();
		void onDeviceReset();
		void onDeviceRestored();
		bool present();

		// GfxRenderWindow implementation
		void resizeWindow(int w=-1, int h=-1);
		bool handlePendingResize();
		void setAsMainWindow();
		size_t getWidth() const;
		size_t getHeight() const;
		void* getHwnd() const;

		bool queryInterface(unsigned __int64 IID, void** ppInterface);

		static HWND createWindow(
			const char* name = "Render Window",
			int x = CW_USEDEFAULT,
			int y = CW_USEDEFAULT,
			int w = CW_USEDEFAULT, 
			int h = CW_USEDEFAULT,
			GfxRenderWindowD3D9* pWindow=0);
			
			DECLARE_GFX_ALLOCATOR();

	private:
		bool createDevice(GfxRenderConfig& cfg, SurfaceFormat fmt);
	};
}

#endif // GFXRENDERWINDOWD3D9_INCLUDED
