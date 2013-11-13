/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(GFXRENDERTARGETD3D9_INCLUDED)
#define GFXRENDERTARGETD3D9_INCLUDED

struct IDirect3DSurface9;

namespace Teardrop
{
	class GfxRenderTarget;

	class GfxRenderTargetD3D9
	{
	public:
		static const unsigned __int64 IID;

		virtual IDirect3DSurface9* getRenderSurface() = 0;
		virtual IDirect3DSurface9* getDepthStencil() = 0;
		virtual void onDeviceReset() = 0;
		virtual void onDeviceRestored() = 0;
		virtual bool present() = 0;

	protected:
		GfxRenderTargetD3D9();
	private:
		GfxRenderTargetD3D9(const GfxRenderTargetD3D9& other);
		GfxRenderTargetD3D9& operator=(const GfxRenderTargetD3D9& other);
	};
}

#endif // GFXRENDERTARGETD3D9_INCLUDED
