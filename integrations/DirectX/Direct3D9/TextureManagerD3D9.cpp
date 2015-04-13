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

#include "stdafx.h"
#include "TextureManagerD3D9.h"
#include "Texture2DD3D9.h"
#include "Asset/TextureAsset.h"
#include <assert.h>

namespace Teardrop {
namespace Gfx {
namespace Direct3D9 {

TextureManager::TextureManager(IDirect3DDevice9* device)
	: mDevice(device)
{
	assert(mDevice);
}

TextureManager::~TextureManager()
{
	for (Tex2DMap::iterator it = mTex2DMap.begin(); it != mTex2DMap.end(); ++it) {
		delete it->second;
	}

	for (RenderTextures::iterator it = mRenderTextures.begin(); it != mRenderTextures.end(); ++it) {
		delete *it;
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
	Direct3D9::Texture2D* tex = TD_NEW Direct3D9::Texture2D(mDevice, texAsset);
	tex->initialize(Texture2D::USAGE_STATIC);
	mTex2DMap[texAsset->getObjectId()] = tex;

	return tex;
}

Gfx::Texture2D* TextureManager::createRenderTexture(TextureAsset* proxyAsset)
{
	Direct3D9::Texture2D* tex = TD_NEW Direct3D9::Texture2D(mDevice, proxyAsset);
	mRenderTextures.push_back(tex);

	return tex;
}

} // namespace Direct3D9
} // namespace Gfx
} // namespace Teardrop
