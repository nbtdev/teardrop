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

#if !defined(TEXTURE2D_D3D11_INCLUDED)
#define TEXTURE2D_D3D11_INCLUDED

#include "stdafx.h"
#include "Gfx/Texture2D.h"

namespace Teardrop
{
	namespace Gfx
	{
		namespace Direct3D11
		{
			class Texture2D : public Gfx::Texture2D
			{
			public:
				Texture2D(ComPtr<ID3D11Device> aDevice, TextureAsset* aAsset, Usage aUsage);
				~Texture2D();

				ComPtr<ID3D11Texture2D> textureObject();
				ComPtr<ID3D11ShaderResourceView> shaderResourceView();
				DXGI_FORMAT textureFormat();

				TD_DECLARE_ALLOCATOR();

			protected:
				ComPtr<ID3D11Device> mDevice;
				ComPtr<ID3D11Texture2D> mTexObject;
				ComPtr<ID3D11ShaderResourceView> mTexRV;
				DXGI_FORMAT mD3D11Format = DXGI_FORMAT_UNKNOWN;
			};
		}
	}
}

#endif // TEXTURE2D_D3D11_INCLUDED
