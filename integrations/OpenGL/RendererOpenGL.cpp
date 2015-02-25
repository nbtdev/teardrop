/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "RendererOpenGL.h"
#include "RenderWindowOpenGL.h"
#include "Util/UUID.h"
#include "Util/_String.h"
#include <X11/Xlib.h>
#include <assert.h>

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

Gfx::RenderTarget*
Renderer::initialize(uintptr_t hWnd, int flags)
{
    OpenGL::RenderWindow* renderWindow = TD_NEW OpenGL::RenderWindow(XOpenDisplay(0), (Window)hWnd, flags);
    mRenderTargets.push_back(renderWindow);

    return renderWindow;
}

void
Renderer::shutdown()
{

}

Gfx::RenderTarget*
Renderer::createRenderWindow(uintptr_t hWnd, SurfaceFormat fmt, int flags)
{
    return nullptr;
}

Gfx::RenderTarget*
Renderer::createRenderTexture(int w, int h, SurfaceFormat fmt, int flags)
{
    return nullptr;
}

void
Renderer::releaseRenderTarget(Gfx::RenderTarget* rt)
{

}

void
Renderer::setRenderTarget(Gfx::RenderTarget* aRT)
{
    // let user clear current RT if they wish
    if (aRT == nullptr) {
        mCurrentRT = nullptr;
        return;
    }

    // and make sure it's one of ours
    for (auto rt : mRenderTargets) {
        if (rt == aRT) {
            mCurrentRT = aRT;
            aRT->setCurrent();
            break;
        }
    }
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
    if (mCurrentRT) {
        mCurrentRT->clear(color, clearColor, depth, depthValue, stencil, stencilValue);
    }
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
    assert(mCurrentRT);
    if (mCurrentRT) {
        mCurrentRT->present();
    }
}

} // namespace OpenGL
} // namespace Gfx
} // namespace Teardrop
