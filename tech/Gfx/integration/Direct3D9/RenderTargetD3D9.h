/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RENDERTARGETD3D9_INCLUDED)
#define RENDERTARGETD3D9_INCLUDED

#include "Gfx/RenderTarget.h"

namespace Teardrop
{
	namespace Gfx 
	{
		namespace Direct3D9
		{
			class RenderTarget : public Gfx::RenderTarget
			{
			public:
				RenderTarget(IDirect3DDevice9* device);
				~RenderTarget();

				IDirect3DSurface9* surface();
				IDirect3DSurface9* depthStencil();

				void setSurface(IDirect3DSurface9* surface);
				void setDepthStencil(IDirect3DSurface9* depthStencil);

				TD_DECLARE_ALLOCATOR();

			protected:
				IDirect3DDevice9* mDevice;
				IDirect3DSurface9* mSurface;
				IDirect3DSurface9* mDepthStencil;
			};
		}
	}
}

#endif // RENDERTARGETD3D9_INCLUDED
