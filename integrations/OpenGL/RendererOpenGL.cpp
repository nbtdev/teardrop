/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "RendererOpenGL.h"
#include "Util/UUID.h"
#include "Util/_String.h"

namespace Teardrop
{
namespace Gfx
{
namespace OpenGL
{

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

RenderTarget*
Renderer::initialize(uintptr_t hWnd, int flags)
{
    return nullptr;
}

void
Renderer::shutdown()
{

}

RenderTarget*
Renderer::createRenderWindow(uintptr_t hWnd, SurfaceFormat fmt, int flags)
{
    return nullptr;
}

RenderTarget*
Renderer::createRenderTexture(int w, int h, SurfaceFormat fmt, int flags)
{
    return nullptr;
}

void
Renderer::releaseRenderTarget(RenderTarget* rt)
{

}

void
Renderer::setRenderTarget(RenderTarget* rt)
{

}

void
Renderer::beginFrame(
    bool color,
    unsigned int clearColor,
    bool depth,
    float depthValue,
    bool stencil,
    unsigned int stencilValue)
{

}

void
Renderer::beginScene(Camera* camera, Viewport*)
{

}

void
Renderer::beginObject(const Matrix44& worldXf)
{

}

void
Renderer::apply(Material* material)
{

}

void
Renderer::render(Submesh* submesh)
{

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

} // namespace OpenGL
} // namespace Gfx
} // namespace Teardrop
