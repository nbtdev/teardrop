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
