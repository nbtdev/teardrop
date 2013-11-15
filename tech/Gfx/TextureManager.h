/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TEXTUREMANAGER_INCLUDED)
#define TEXTUREMANAGER_INCLUDED

#include "Memory/Allocators.h"
#include "Core/Singleton.h"

namespace Teardrop
{
	class TextureAsset;

	namespace Gfx
	{
		class Texture2D;

		class TextureManager : public Singleton<TextureManager>
		{
		public:
			TextureManager();
			~TextureManager();

			virtual Texture2D* createOrFindInstance2D(TextureAsset* texAsset) = 0;
			virtual Texture2D* createRenderTexture(TextureAsset* proxyAsset) = 0;

			TD_DECLARE_ALLOCATOR();

		protected:
		};
	}
}

#endif // TEXTUREMANAGER_INCLUDED
