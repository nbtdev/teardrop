/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "ShaderManagerD3D9.h"
#include "FragmentShaderD3D9.h"
#include "VertexShaderD3D9.h"
#include "Gfx/Material.h"
#include "Gfx/Submesh.h"
#include "Gfx/ShaderFeatures.h"
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
	for (FragmentShaders::iterator it = mFragmentShaders.begin(); it != mFragmentShaders.end(); ++it) {
		it->second->destroy();
		delete it->second;
	}
}

Gfx::FragmentShader* ShaderManager::createOrFindInstanceOf(Material* material)
{
	FragmentShaders::iterator it = mFragmentShaders.find(material->getObjectId());
	if (it != mFragmentShaders.end()) 
		return it->second;

	// else, create and record a new one
	Direct3D9::FragmentShader* shader = TD_NEW Direct3D9::FragmentShader(mDevice, material);
	shader->initialize();
	mFragmentShaders[material->getObjectId()] = shader;

	return shader;
}

Gfx::VertexShader* ShaderManager::createOrFindInstanceOf(Submesh* submesh)
{
	assert(submesh);
	if (!submesh)
		return 0;

	VertexShaders::iterator it = mVertexShaders.find(submesh->hash());
	if (it != mVertexShaders.end()) 
		return it->second;

	// else, create and record a new one
	Direct3D9::VertexShader* shader = TD_NEW Direct3D9::VertexShader(mDevice);
	shader->initialize(submesh);
	mVertexShaders[submesh->hash()] = shader;

	return shader;
}

void ShaderManager::release(Gfx::FragmentShader* shader)
{
	// TODO: refcounting?
	mFragmentShaders.erase(shader->materialId());
	shader->destroy();
	delete shader;
}

void ShaderManager::release(Gfx::VertexShader* shader)
{
	// TODO: refcounting?
	mVertexShaders.erase(shader->hash());
	shader->destroy();
	delete shader;
}

} // namespace Direct3D9
} // namespace Gfx
} // namespace Teardrop
