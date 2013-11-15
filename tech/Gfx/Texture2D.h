/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TEXTURE2D_INCLUDED)
#define TEXTURE2D_INCLUDED

#include "Memory/Allocators.h"

namespace Teardrop
{
	class TextureAsset;

	namespace Gfx
	{
		class Texture2D
		{
		public:
			Texture2D(TextureAsset* asset);
			virtual ~Texture2D();

			enum Usage {
				USAGE_STATIC, 
				USAGE_DYNAMIC,
				USAGE_RENDERTARGET,
			};

			TD_DECLARE_ALLOCATOR();

		protected:
			TextureAsset* mAsset;
		};
	}
}

#endif // TEXTURE2D_INCLUDED
