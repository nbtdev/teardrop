/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "Asset/TextureAsset.h"
#include "RenderTextureD3D9.h"
#include "TextureManagerD3D9.h"

namespace Teardrop {
namespace Gfx {
namespace Direct3D9 {

RenderTexture::RenderTexture(IDirect3DDevice9* device)
	: RenderTarget(device)
	, mTexture(0)
{
	mProxyAsset = TD_NEW TextureAsset;
}

RenderTexture::~RenderTexture()
{
	delete mProxyAsset;
}

bool RenderTexture::initialize(int w, int h, SurfaceFormat fmt)
{
	mProxyAsset->setWidth(w);
	mProxyAsset->setHeight(h);

	if (fmt == SURFACE_R8G8B8) 
		mProxyAsset->setFormat(TextureAsset::TEXFMT_RGB);
	if (fmt == SURFACE_A8R8G8B8) 
		mProxyAsset->setFormat(TextureAsset::TEXFMT_ARGB);
	if (fmt == SURFACE_RGBA_32F) 
		mProxyAsset->setFormat(TextureAsset::TEXFMT_ARGB_F32);

	mTexture = TextureManager::instance().createRenderTexture(mProxyAsset);

	return true;
}

Texture2D* RenderTexture::texture()
{
	return mTexture;
}

} // namespace Direct3D9
} // namespace Gfx
} // namespace Teardrop
