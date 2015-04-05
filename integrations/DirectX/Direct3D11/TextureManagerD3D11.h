/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TEXTUREMANAGERD3D11_INCLUDED)
#define TEXTUREMANAGERD3D11_INCLUDED

#include "Gfx/TextureManager.h"
#include "Util/UUID.h"
#include <map>
#include <list>

namespace Teardrop
{
	namespace Gfx
	{
		namespace Direct3D11
		{
			class TextureManager : public Teardrop::Gfx::TextureManager
			{
			public:
				TextureManager(ComPtr<ID3D11Device> aDevice);
				~TextureManager();

				Gfx::Texture2D* createOrFindInstance2D(TextureAsset* texAsset);
				Gfx::Texture2D* createRenderTexture(TextureAsset* proxyAsset);

				TD_DECLARE_ALLOCATOR();

			protected:
				ComPtr<ID3D11Device> mDevice;

				typedef std::map<UUID, Texture2D*> Tex2DMap;
				Tex2DMap mTex2DMap;

				typedef std::list<Texture2D*> RenderTextures;
				RenderTextures mRenderTextures;
			};
		}
	}
}

#endif // TEXTUREMANAGERD3D11_INCLUDED
