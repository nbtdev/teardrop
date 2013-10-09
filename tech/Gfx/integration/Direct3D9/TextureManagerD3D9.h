/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TEXTUREMANAGERD3D9_INCLUDED)
#define TEXTUREMANAGERD3D9_INCLUDED

#include "Gfx/TextureManager.h"
#include "Util/UUID.h"
#include <map>

namespace Teardrop
{
	namespace Gfx
	{
		namespace Direct3D9
		{
			class TextureManager : public Teardrop::Gfx::TextureManager
			{
			public:
				TextureManager();
				~TextureManager();

				Texture2D* createOrFindInstance2D(TextureAsset* texAsset);

				TD_DECLARE_ALLOCATOR();

			protected:
				typedef std::map<UUID, Texture2D*> Tex2DMap;
				Tex2DMap mTex2DMap;
			};
		}
	}
}

#endif // TEXTUREMANAGERD3D9_INCLUDED
