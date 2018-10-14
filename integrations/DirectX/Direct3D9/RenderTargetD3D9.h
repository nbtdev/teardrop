/******************************************************************************
Copyright (c) 2018 Teardrop Games

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
******************************************************************************/

#if !defined(RENDERTARGETD3D9_INCLUDED)
#define RENDERTARGETD3D9_INCLUDED

#include "Gfx/RenderTarget.h"
#include "Memory/Allocators.h"
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
				void unsetCurrent();
				Viewport* addViewport(float x = 0, float y = 0, float w = 1, float h = 1, unsigned int zOrder = 0);
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
