/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(GFXRENDERWINDOWD3D9_INCLUDED)
#define GFXRENDERWINDOWD3D9_INCLUDED

#include "GfxRenderWindow.h"
#include "GfxRenderTarget.h"
#include "GfxRenderTargetD3D9.h"

struct IDirect3DSwapChain9;

namespace Teardrop
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
