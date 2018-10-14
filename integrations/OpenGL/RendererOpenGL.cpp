/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "RendererOpenGL.h"
#include "BufferManagerOpenGL.h"
#include "FragmentShaderOpenGL.h"
#include "GLHeaders.h"
#include "ProgramOpenGL.h"
#include "ShaderManagerOpenGL.h"
#include "TextureManagerOpenGL.h"
#include "VertexShaderOpenGL.h"
#include "Gfx/Exception.h"
#include "Gfx/Material.h"
#include "Gfx/RenderTarget.h"
#include "Gfx/Submesh.h"
#include "Gfx/Viewport.h"
#include "Math/Vector2.h"
#include "Util/Hash.h"
#include "Util/UUID.h"
#include "Util/_String.h"
#include <assert.h>

namespace Teardrop {
namespace Gfx {
namespace OpenGL {

Renderer::Renderer(int aFlags)
{
	{
		// we have to create a dummy window in order to load extensions
		std::shared_ptr<RenderTarget> dummy = createDummyContext();

		// now we can initialize extensions
//        glewExperimental = GL_TRUE;
//		if (GLEW_OK != glewInit()) {
//			throw Exception("Could not initialize GLEW in OpenGL::Renderer");
//		}
	}

	// and initialize managers
	TD_NEW BufferManager;
	TD_NEW ShaderManager;
	TD_NEW TextureManager;
}

Renderer::~Renderer()
{
	TextureManager::instance().shutdown();
	ShaderManager::instance().shutdown();
	BufferManager::instance().shutdown();
}

std::shared_ptr<Gfx::RenderTarget>
Renderer::createRenderTexture(int w, int h, SurfaceFormat fmt, int flags)
{
    return nullptr;
}

void
Renderer::beginFrame()
{
}

void
Renderer::beginScene(Camera* /*aCamera*/, Viewport* aVP)
{
    // setup viewport and camera for the next render
    const Vector2& pos = aVP->getPosition();
    const Vector2& dim = aVP->getSize();

	aVP->updateDimensions();
    glViewport(int(pos.x), int(pos.y), int(dim.x), int(dim.y));
}

void
Renderer::beginObject(const Matrix44& worldXf)
{

}

void
Renderer::apply(Material* material)
{
	mCurrentMtl = material;
}

void
Renderer::render(Submesh* submesh)
{
	// calculate the program to use (creating a new one if necessary) based on current material and 
	// this submesh
	std::shared_ptr<Program> program = findOrCreateProgram(mCurrentMtl, submesh);

	if (program != mCurrentProgram) {
		// save off this one and apply it...
		mCurrentProgram = program;

		// sanity check...
		if (mCurrentProgram)
			mCurrentProgram->apply();
	}

	// clean up after this render...
	if (mCurrentProgram)
		mCurrentProgram->disable();
}

void
Renderer::endObject()
{

}

void
Renderer::endScene()
{

}

void
Renderer::endFrame()
{
}

struct ForHash
{
	UUID mtlUuid;
	unsigned int submeshHash = 0;
};

std::shared_ptr<Program>
Renderer::findOrCreateProgram(Material* aMaterial, Submesh* aSubmesh)
{
	// delegate shader management to ShaderManager for now
	Gfx::VertexShader* vs = ShaderManager::instance().createOrFindInstanceOf(aSubmesh);
	Gfx::FragmentShader* fs = ShaderManager::instance().createOrFindInstanceOf(aMaterial);

	ForHash forHash;
	forHash.mtlUuid = aMaterial->getObjectId();
	forHash.submeshHash = aSubmesh->hash();
	uint64_t key = hashData64(&forHash, sizeof(forHash));

	std::shared_ptr<Program> program;

	auto it = mPrograms.find(key);
	if (it == mPrograms.end()) {
		// need to make a new one...
		program.reset(TD_NEW Program);

		if (program->initialize(static_cast<VertexShader*>(vs), static_cast<FragmentShader*>(fs))) {
			// ...and add it to the table
			mPrograms[key] = program;
		}

		// if the initialize failed, the SP will go out of scope and take the program with it; 
		// otherwise, the mPrograms table will be holding a reference to it, keeping the SP good
	}

	return program;
}

} // namespace OpenGL
} // namespace Gfx
} // namespace Teardrop
