/******************************************************************************
Copyright (c) 2015 Teardrop Games

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
