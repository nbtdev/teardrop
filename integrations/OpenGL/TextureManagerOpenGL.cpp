/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "TextureManagerOpenGL.h"
#include "Texture2DOpenGL.h"
#include "Asset/TextureAsset.h"
#include <assert.h>

namespace Teardrop {
namespace Gfx {
namespace OpenGL {

TextureManager::TextureManager()
{
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
	OpenGL::Texture2D* tex = TD_NEW OpenGL::Texture2D(texAsset);
	tex->initialize(Texture2D::USAGE_STATIC);
	mTex2DMap[texAsset->getObjectId()] = tex;

	return tex;
}

Gfx::Texture2D* TextureManager::createRenderTexture(TextureAsset* proxyAsset)
{
	OpenGL::Texture2D* tex = TD_NEW OpenGL::Texture2D(proxyAsset);
	mRenderTextures.push_back(tex);

	return tex;
}

} // namespace OpenGL
} // namespace Gfx
} // namespace Teardrop
