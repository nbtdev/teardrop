/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(GFXTEXTURED3D9_INCLUDED)
#define GFXTEXTURED3D9_INCLUDED

#include "GfxTexture.h"

namespace Teardrop
{
	struct Environment;

	class GfxTextureD3D9 : public GfxTexture
	{
		DECLARE_SERIALIZABLE(GfxTextureD3D9);

		SerialPointer<IDirect3DTexture9> m_pTex;

	public:
		GfxTextureD3D9();
		GfxTextureD3D9(int);
		~GfxTextureD3D9();

		bool initialize(
			unsigned int width, 
			unsigned int height, 
			unsigned int depth, 
			Format format, 
			Type type, 
			size_t bDynamic,
			unsigned int mips=0, 
			void* pData=0,
			unsigned int dataLen=0);

		bool destroy();
		bool release();
		void releaseD3DResources();
		bool delayInit();
		void* lock(int& pitch, bool bDiscardExisting=true);
		void unlock();
		IDirect3DTexture9* getD3D9Texture();
		bool setToSampler(size_t sampler);

		bool serialize(ResourceSerializer& serializer);

		DECLARE_GFX_ALLOCATOR();

	private:
		bool initD3D9Tex(size_t bDynamic);
	};
}

#endif // GFXTEXTURED3D9_INCLUDED
