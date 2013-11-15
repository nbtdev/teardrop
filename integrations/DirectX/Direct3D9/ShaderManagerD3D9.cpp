/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "ShaderManagerD3D9.h"
#include "ShaderD3D9.h"
#include "Gfx/Material.h"
#include <assert.h>

namespace Teardrop {
namespace Gfx {
namespace Direct3D9 {

ShaderManager::ShaderManager(IDirect3DDevice9* device)
	: mDevice(device)
{
	assert(mDevice);
}

ShaderManager::~ShaderManager()
{
	for (Shaders::iterator it = mShaders.begin(); it != mShaders.end(); ++it) {
		it->second->destroy();
		delete it->second;
	}
}

Gfx::Shader* ShaderManager::createOrFindInstanceOf(Material* material)
{
	Shaders::iterator it = mShaders.find(material->getObjectId());
	if (it != mShaders.end()) 
		return it->second;

	// else, create and record a new one
	Direct3D9::Shader* shader = TD_NEW Direct3D9::Shader(mDevice, material);
	shader->initialize();
	mShaders[material->getObjectId()] = shader;

	return shader;
}

void ShaderManager::release(Gfx::Shader* shader)
{
	// TODO: refcounting?
	mShaders.erase(shader->materialId());
	shader->destroy();
	delete shader;
}

} // namespace Direct3D9
} // namespace Gfx
} // namespace Teardrop
