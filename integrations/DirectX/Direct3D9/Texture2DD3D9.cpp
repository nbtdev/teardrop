/******************************************************************************
Copyright (c) 2018 Teardrop Games

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
******************************************************************************/

#include "stdafx.h"
#include "Texture2DD3D9.h"
#include "Asset/TextureAsset.h"
#include <assert.h>

namespace Teardrop {
namespace Gfx {
namespace Direct3D9 {

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

Texture2D::Texture2D(IDirect3DDevice9* device, TextureAsset* asset)
	: Gfx::Texture2D(asset)
	, mDevice(device)
	, mTexObject(0)
{
	assert(mDevice);
	if (mDevice)
		mDevice->AddRef();
}

bool Texture2D::initialize(Usage kUsage)
{
	assert(mTexObject == 0);

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

	return true;
}

Texture2D::~Texture2D()
{
	if (mTexObject) {
		mTexObject->Release();
		mTexObject = 0;
	}

	if (mDevice)
		mDevice->Release();
}

IDirect3DTexture9* Texture2D::textureObject()
{
	return mTexObject;
}

} // Direct3D9
} // Gfx
} // Teardrop
