/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(GFXRENDERTEXTURE_INCLUDED)
#define GFXRENDERTEXTURE_INCLUDED

namespace Teardrop
{
	class GfxViewport;

	class GfxRenderTexture
	{
	public:
		static const unsigned __int64 IID;

		virtual void* getTextureHandle() const = 0;
		virtual GfxViewport* getFullTargetViewport() const = 0;

	protected:
		GfxRenderTexture();
	private:
		GfxRenderTexture(const GfxRenderTexture& other);
		GfxRenderTexture& operator=(const GfxRenderTexture& other);
	};
}

#endif // GFXRENDERTEXTURE_INCLUDED
