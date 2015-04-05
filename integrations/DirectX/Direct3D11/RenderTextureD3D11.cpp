/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "RenderTextureD3D11.h"
#include "Texture2DD3D11.h"
#include "TextureManagerD3D11.h"
#include "Asset/TextureAsset.h"

namespace Teardrop {
namespace Gfx {
namespace Direct3D11 {

RenderTexture::RenderTexture(ComPtr<ID3D11Device> aDevice, int aWidth, int aHeight, SurfaceFormat aFormat)
	: RenderTarget(aDevice, aWidth, aHeight)
	, mTexture(nullptr)
{
	mProxyAsset = TD_NEW TextureAsset;
	mProxyAsset->setWidth(aWidth);
	mProxyAsset->setHeight(aHeight);

	if (aFormat == SURFACE_R8G8B8)
		mProxyAsset->setFormat(TextureAsset::TEXFMT_RGB);
	if (aFormat == SURFACE_A8R8G8B8)
		mProxyAsset->setFormat(TextureAsset::TEXFMT_ARGB);
	if (aFormat == SURFACE_RGBA_32F)
		mProxyAsset->setFormat(TextureAsset::TEXFMT_ARGB_F32);

	mTexture = static_cast<Texture2D*>(TextureManager::instance().createRenderTexture(mProxyAsset));
	assert(mTexture);

	// create render target view from the texture object
	ID3D11Texture2D* tex = mTexture->textureObject().Get();
	CD3D11_RENDER_TARGET_VIEW_DESC desc(tex, D3D11_RTV_DIMENSION_TEXTURE2D);
	HRESULT hr = aDevice->CreateRenderTargetView(tex, &desc, &mRTView);
}

RenderTexture::~RenderTexture()
{
	delete mProxyAsset;
	delete mTexture;
}

void RenderTexture::resize(int w, int h)
{

}

Texture2D* RenderTexture::texture()
{
	return mTexture;
}

} // namespace Direct3D11
} // namespace Gfx
} // namespace Teardrop
