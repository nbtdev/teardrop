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

#if !defined(RENDERTARGETD3D11_INCLUDED)
#define RENDERTARGETD3D11_INCLUDED

#include "stdafx.h"
#include "Gfx/RenderTarget.h"
#include "Memory/Allocators.h"
#include <map>

namespace Teardrop
{
	namespace Gfx
	{
		namespace Direct3D11
		{
			class Renderer;

			class RenderTarget : public Gfx::RenderTarget
			{
			public:
				RenderTarget(ComPtr<ID3D11Device> aDevice, int aWidth, int aHeight);
				~RenderTarget();

				void clear(
					bool color = true,
					unsigned int clearColor = 0,
					bool depth = true,
					float depthValue = 1,
					bool stencil = true,
					unsigned int stencilValue = 0);
				float aspect();
				int width();
				int height();
				void setCurrent();
				void unsetCurrent();
				Gfx::Viewport* addViewport(float x = 0, float y = 0, float w = 1, float h = 1, unsigned int zOrder = 0);
				void releaseViewport(Gfx::Viewport* vp);

				TD_DECLARE_ALLOCATOR();

			protected:
				ComPtr<ID3D11Device> mDevice;
				ComPtr<ID3D11DeviceContext> mDeviceContext;
				ComPtr<ID3D11BlendState> mBlendState;
				ComPtr<ID3D11DepthStencilState> mDepthStencilState;
				ComPtr<ID3D11RenderTargetView> mRenderTargetView;
				ComPtr<ID3D11Texture2D> mDepthStencilTexture;
				ComPtr<ID3D11DepthStencilView> mDepthStencilView;

				int mWidth = 0;
				int mHeight = 0;

				typedef std::multimap<unsigned int /*zOrder*/, Viewport*> Viewports;
				Viewports mViewports;
			};
		}
	}
}

#endif // RENDERTARGETD3D11_INCLUDED
