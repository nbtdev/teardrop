/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RENDERTARGETD3D11_INCLUDED)
#define RENDERTARGETD3D11_INCLUDED

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
