/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RENDERTEXTURED3D11_INCLUDED)
#define RENDERTEXTURED3D11_INCLUDED

#include "RenderTargetD3D11.h"
#include "Gfx/Common.h"

namespace Teardrop
{
	class TextureAsset;

	namespace Gfx
	{
		namespace Direct3D11
		{
			class Texture2D;

			class RenderTexture : public RenderTarget
			{
			public:
				RenderTexture(ComPtr<ID3D11Device> aDevice, int aWidth, int aHeight, SurfaceFormat aFormat);
				~RenderTexture();

				void resize(int w, int h);

				Texture2D* texture();

				TD_DECLARE_ALLOCATOR();

			protected:
				ComPtr<ID3D11Device> mDevice;
				ComPtr<ID3D11RenderTargetView> mRTView;
				Texture2D* mTexture = nullptr;
				TextureAsset* mProxyAsset = nullptr;
			};
		}
	}
}

#endif // RENDERTEXTURED3D11_INCLUDED
