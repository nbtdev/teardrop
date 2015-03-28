/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Texture2DOpenGL.h"
#include "Asset/TextureAsset.h"
#include <assert.h>

namespace Teardrop {
namespace Gfx {
namespace OpenGL {

#if 0
	static D3DFORMAT sLut[] = {
	D3DFMT_UNKNOWN,			// TEXFMT_R
	D3DFMT_UNKNOWN,			// TEXFMT_RG
	D3DFMT_R8G8B8,			// TEXFMT_RGB
	D3DFMT_X8R8G8B8,		// TEXFMT_ARGB
	D3DFMT_R32F,			// TEXFMT_R_F32
	D3DFMT_G32R32F,			// TEXFMT_RG_F32
	D3DFMT_UNKNOWN,			// TEXFMT_RGB_F32
	D3DFMT_A32B32G32R32F,	// TEXFMT_RGBA_F32
	D3DFMT_DXT1,			// TEXFMT_BC1
	D3DFMT_DXT3,			// TEXFMT_BC2
	D3DFMT_DXT5,			// TEXFMT_BC3
	D3DFMT_UNKNOWN,			// TEXFMT_PVRTC
};
#endif

Texture2D::Texture2D(TextureAsset* asset)
	: Gfx::Texture2D(asset)
	, mTexObject(0)
{
}

bool Texture2D::initialize(Usage kUsage)
{
	assert(mTexObject == 0);
#if 0
	// create texture object
	HRESULT hr;

	DWORD usage = 0;
	if (kUsage == USAGE_RENDERTARGET)
		usage = D3DUSAGE_RENDERTARGET;
	else
		if (kUsage == USAGE_DYNAMIC)
			usage = D3DUSAGE_DYNAMIC;

	bool genMips = mAsset->getGenerateMipmaps();

	// because "0" actually means "generate them all", when what we really 
	// meant was "generate nothing beyond the first one", which would be "1"
	UINT actualMips = genMips ? 0 : 1;

	// use D3DX to load DDS textures, another way otherwise
	TextureAsset::Format fmt = mAsset->getFormat();
	//if (fmt < TextureAsset::TEXFMT_BC1 || fmt > TextureAsset::TEXFMT_BC3)
	//{
	hr = mDevice->CreateTexture(
		(UINT)mAsset->getWidth(),
		(UINT)mAsset->getHeight(),
		actualMips,
		usage,
		sLut[fmt],
		D3DPOOL_MANAGED,
		&mTexObject,
		0);

	// copy data to the new texture
	D3DLOCKED_RECT rect;
	mTexObject->LockRect(0, &rect, 0, 0);
	void* dest = rect.pBits;
	const void* src = mAsset->data();
	int len = mAsset->length();
	memcpy(dest, src, len);
	mTexObject->UnlockRect(0);
	//}
	//else
	//{
	//hr = D3DXCreateTextureFromFileInMemoryEx(
	//	mDevice,
	//	mAsset->data(),
	//	(UINT)mAsset->length(),
	//	D3DX_DEFAULT,
	//	D3DX_DEFAULT,
	//	(UINT)actualMips,
	//	0,
	//	sLut[fmt],
	//	D3DPOOL_DEFAULT,
	//	D3DX_DEFAULT,
	//	D3DX_DEFAULT,
	//	0,
	//	0,
	//	0,
	//	&mTexObject);
	//}

	if (FAILED(hr))
	{
		// TODO: log
		return false;
	}
#endif
	return true;
}

Texture2D::~Texture2D()
{
#if 0
	if (mTexObject) {
		mTexObject->Release();
		mTexObject = 0;
	}
#endif
}

GLuint Texture2D::textureName()
{
	return mTexObject;
}

} // OpenGL
} // Gfx
} // Teardrop
