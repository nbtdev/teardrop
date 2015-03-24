/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "RendererOpenGL.h"
#include "ExtensionManager.h"
#include "Gfx/RenderTarget.h"
#include "Gfx/Viewport.h"
#include "Math/Vector2.h"
#include "Util/UUID.h"
#include "Util/_String.h"
#include <assert.h>

namespace Teardrop {
namespace Gfx {
namespace OpenGL {

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

Gfx::RenderTarget*
Renderer::initialize(uintptr_t hWnd, int flags)
{
	Gfx::RenderTarget* rt = createRenderWindow(hWnd, SURFACE_A8R8G8B8, flags);

    // now we can initialize extensions
    mExtMgr = TD_NEW ExtensionManager;

    return rt;
}

void
Renderer::shutdown()
{
    delete mExtMgr;
    mExtMgr = nullptr;
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
Renderer::beginScene(Camera* /*aCamera*/, Viewport* aVP)
{
    // setup viewport and camera for the next render
    const Vector2& pos = aVP->getPosition();
    const Vector2& dim = aVP->getSize();

    glViewport(int(pos.x), int(pos.y), int(dim.x), int(dim.y));
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
