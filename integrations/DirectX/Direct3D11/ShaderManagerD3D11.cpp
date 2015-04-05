/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "ShaderManagerD3D11.h"
#include "FragmentShaderD3D11.h"
#include "VertexShaderD3D11.h"
#include "Gfx/Material.h"
#include "Gfx/Submesh.h"
#include "Gfx/ShaderFeatures.h"
#include <assert.h>

namespace Teardrop {
namespace Gfx {
namespace Direct3D11 {

ShaderManager::ShaderManager(ComPtr<ID3D11Device> aDevice)
	: mDevice(aDevice)
{
	assert(mDevice);
}

ShaderManager::~ShaderManager()
{
	for (auto s : mFragmentShaders) {
		delete s.second;
	}

	for (auto s : mVertexShaders) {
		delete s.second;
	}
}

Gfx::FragmentShader* ShaderManager::createOrFindInstanceOf(Material* material)
{
	auto it = mFragmentShaders.find(material->getObjectId());
	if (it != mFragmentShaders.end())
		return it->second;

	// else, create and record a new one
	FragmentShader* shader = TD_NEW FragmentShader(mDevice, mConstants, material);
	mFragmentShaders[material->getObjectId()] = shader;

	return shader;
}

Gfx::VertexShader* ShaderManager::createOrFindInstanceOf(Submesh* submesh)
{
	assert(submesh);
	if (!submesh)
		return 0;

	auto it = mVertexShaders.find(submesh->hash());
	if (it != mVertexShaders.end())
		return it->second;

	// else, create and record a new one
	VertexShader* shader = TD_NEW VertexShader(mDevice, mConstants, submesh);
	mVertexShaders[submesh->hash()] = shader;

	return shader;
}

void ShaderManager::release(Gfx::FragmentShader* shader)
{
	// TODO: refcounting?
	mFragmentShaders.erase(shader->materialId());
	delete shader;
}

void ShaderManager::release(Gfx::VertexShader* shader)
{
	// TODO: refcounting?
	mVertexShaders.erase(shader->hash());
	delete shader;
}

} // namespace Direct3D11
} // namespace Gfx
} // namespace Teardrop
