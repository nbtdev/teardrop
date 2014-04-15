/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RENDERTEXTURED3D9_INCLUDED)
#define RENDERTEXTURED3D9_INCLUDED

#include "RenderTargetD3D9.h"
#include "Gfx/Common.h"

namespace Teardrop
{
	class TextureAsset;

	namespace Gfx 
	{
		class Texture2D;

		namespace Direct3D9
		{
			class RenderTexture : public RenderTarget
			{
			public:
				RenderTexture(IDirect3DDevice9* device);
				~RenderTexture();

				bool initialize(int w, int h, SurfaceFormat fmt);
				void resize(int w, int h);

				Texture2D* texture();

				TD_DECLARE_ALLOCATOR();

			protected:
				Texture2D* mTexture;
				TextureAsset* mProxyAsset;
			};
		}
	}
}

#endif // RENDERTEXTURED3D9_INCLUDED
