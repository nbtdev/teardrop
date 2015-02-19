/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(GFXRENDERTEXTURE_INCLUDED)
#define GFXRENDERTEXTURE_INCLUDED

#include <cstdint>

namespace Teardrop
{
	class GfxViewport;

	class GfxRenderTexture
	{
	public:
        GfxRenderTexture(const GfxRenderTexture& other) = delete;
        GfxRenderTexture& operator=(const GfxRenderTexture& other) = delete;

        static const uint64_t IID;

		virtual void* getTextureHandle() const = 0;
		virtual GfxViewport* getFullTargetViewport() const = 0;

	protected:
		GfxRenderTexture();
	};
}

#endif // GFXRENDERTEXTURE_INCLUDED
