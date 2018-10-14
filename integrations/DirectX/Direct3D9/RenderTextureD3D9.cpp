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

void RenderTexture::resize(int w, int h)
{

}

Texture2D* RenderTexture::texture()
{
	return mTexture;
}

} // namespace Direct3D9
} // namespace Gfx
} // namespace Teardrop
