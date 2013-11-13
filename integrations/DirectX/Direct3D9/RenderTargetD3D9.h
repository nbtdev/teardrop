/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RENDERTARGETD3D9_INCLUDED)
#define RENDERTARGETD3D9_INCLUDED

#include "Gfx/RenderTarget.h"
#include <map>

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

				void clear(
					bool color = true,
					unsigned int clearColor = 0,
					bool depth = true,
					float depthValue = 1,
					bool stencil = true,
					unsigned int stencilValue = 0);
				int width();
				int height();
				void setCurrent();
				Viewport* addViewport(float x=0, float y=0, float w=1, float h=1, unsigned int zOrder=0);
				void releaseViewport(Viewport* vp);

				IDirect3DSurface9* surface();
				IDirect3DSurface9* depthStencil();

				void setSurface(IDirect3DSurface9* surface);
				void setDepthStencil(IDirect3DSurface9* depthStencil);

				TD_DECLARE_ALLOCATOR();

			protected:
				IDirect3DDevice9* mDevice;
				IDirect3DSurface9* mSurface;
				IDirect3DSurface9* mDepthStencil;
				int mWidth;
				int mHeight;

				typedef std::multimap<unsigned int /*zOrder*/, Viewport*> Viewports;
				Viewports mViewports;
			};
		}
	}
}

#endif // RENDERTARGETD3D9_INCLUDED
