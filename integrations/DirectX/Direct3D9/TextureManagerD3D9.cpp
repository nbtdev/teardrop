/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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