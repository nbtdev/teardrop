/******************************************************************************
Copyright (c) 2015 Teardrop Games

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

#if !defined(TEXTUREMANAGERD3D11_INCLUDED)
#define TEXTUREMANAGERD3D11_INCLUDED

#include "stdafx.h"
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
