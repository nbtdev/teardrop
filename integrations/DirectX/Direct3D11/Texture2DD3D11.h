/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TEXTURE2D_D3D11_INCLUDED)
#define TEXTURE2D_D3D11_INCLUDED

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
				DXGI_FORMAT textureFormat();

				TD_DECLARE_ALLOCATOR();

			protected:
				ComPtr<ID3D11Device> mDevice;
				ComPtr<ID3D11Texture2D> mTexObject;
				DXGI_FORMAT mD3D11Format = DXGI_FORMAT_UNKNOWN;
			};
		}
	}
}

#endif // TEXTURE2D_D3D11_INCLUDED
