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

#include "TextureManagerD3D11.h"
#include "Texture2DD3D11.h"
#include "Asset/TextureAsset.h"
#include <assert.h>

namespace Teardrop {
namespace Gfx {
namespace Direct3D11 {

TextureManager::TextureManager(ComPtr<ID3D11Device> aDevice)
	: mDevice(aDevice)
{
	assert(mDevice);
}

TextureManager::~TextureManager()
{
	for (auto pr : mTex2DMap) {
		delete pr.second;
	}

	for (auto rt : mRenderTextures) {
		delete rt;
	}
}

Gfx::Texture2D* TextureManager::createOrFindInstance2D(TextureAsset* texAsset)
{
	assert(texAsset);
	if (!texAsset)
		return 0;

	Tex2DMap::iterator it = mTex2DMap.find(texAsset->getObjectId());
	if (it != mTex2DMap.end())
		return it->second;

	// else, create and record a new one
	Texture2D* tex = TD_NEW Texture2D(mDevice, texAsset, Texture2D::USAGE_STATIC);
	mTex2DMap[texAsset->getObjectId()] = tex;

	return tex;
}

Gfx::Texture2D* TextureManager::createRenderTexture(TextureAsset* proxyAsset)
{
	Texture2D* tex = TD_NEW Texture2D(mDevice, proxyAsset, Texture2D::USAGE_RENDERTARGET);
	mRenderTextures.push_back(tex);

	return tex;
}

} // namespace Direct3D11
} // namespace Gfx
} // namespace Teardrop
