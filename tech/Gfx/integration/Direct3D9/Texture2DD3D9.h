/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TEXTURE2D_D3D9_INCLUDED)
#define TEXTURE2D_D3D9_INCLUDED

#include "Gfx/Texture2D.h"

namespace Teardrop
{
	namespace Gfx
	{
		namespace Direct3D9 
		{
			class Texture2D : public Gfx::Texture2D
			{
			public:
				Texture2D(TextureAsset* asset);
				~Texture2D();

				bool initialize(IDirect3DDevice9* device, Usage usage);
				IDirect3DTexture9* textureObject();

				TD_DECLARE_ALLOCATOR();

			protected:
				IDirect3DTexture9* mTexObject;
			};
		}
	}
}

#endif // TEXTURE2D_D3D9_INCLUDED
