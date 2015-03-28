/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TEXTURE2D_OPENGL_INCLUDED)
#define TEXTURE2D_OPENGL_INCLUDED

#include "Gfx/Texture2D.h"
#include "GLHeaders.h"

namespace Teardrop
{
	namespace Gfx
	{
		namespace OpenGL
		{
			class Texture2D : public Gfx::Texture2D
			{
			public:
				Texture2D(TextureAsset* asset);
				~Texture2D();

				bool initialize(Usage usage);
				GLuint textureName();

				TD_DECLARE_ALLOCATOR();

			protected:
				GLuint mTexObject;
			};
		}
	}
}

#endif // TEXTURE2D_OPENGL_INCLUDED
