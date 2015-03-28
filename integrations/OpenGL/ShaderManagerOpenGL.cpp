/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "ShaderManagerOpenGL.h"
#include "FragmentShaderOpenGL.h"
#include "VertexShaderOpenGL.h"
#include "Gfx/Material.h"
#include "Gfx/Submesh.h"
#include "Gfx/ShaderFeatures.h"
#include <assert.h>

namespace Teardrop {
namespace Gfx {
namespace OpenGL {

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
	for (auto fs : mFragmentShaders) {
		fs.second->destroy();
		delete fs.second;
	}

	for (auto vs : mVertexShaders) {
		vs.second->destroy();
		delete vs.second;
	}
}

Gfx::FragmentShader* 
ShaderManager::createOrFindInstanceOf(Material* material)
{
	FragmentShaders::iterator it = mFragmentShaders.find(material->getObjectId());
	if (it != mFragmentShaders.end())
		return it->second;

	// else, create and record a new one
	OpenGL::FragmentShader* shader = TD_NEW OpenGL::FragmentShader(mConstants, material);
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
	OpenGL::VertexShader* shader = TD_NEW OpenGL::VertexShader(mConstants);
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
