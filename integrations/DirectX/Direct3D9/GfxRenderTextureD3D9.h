/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(GFXRENDERTEXTURED3D9_INCLUDED)
#define GFXRENDERTEXTURED3D9_INCLUDED

#include "GfxRenderTexture.h"
#include "GfxRenderTarget.h"
#include "GfxRenderTargetD3D9.h"

namespace Teardrop
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
