/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
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
