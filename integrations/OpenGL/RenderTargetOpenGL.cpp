/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "RenderTargetOpenGL.h"
#include "ViewportOpenGL.h"
#include "Math/Vector2.h"
#include <assert.h>
#include <GL/gl.h>

namespace Teardrop {
namespace Gfx {
namespace OpenGL {

RenderTarget::RenderTarget()
{
}

RenderTarget::~RenderTarget()
{
    for (Viewports::iterator it = mViewports.begin(); it != mViewports.end(); ++it) {
        delete it->second;
    }
}

void RenderTarget::clear(
    bool color /* = true */, unsigned int clearColor /* = 0 */,
    bool depth /* = true */, float depthValue /* = 1 */,
    bool stencil /* = true */, unsigned int stencilValue /* = 0 */)
{
    GLbitfield flags = 0;
    if (color) {
        flags |= GL_COLOR_BUFFER_BIT;

        float red = float(uint8_t(clearColor >> 24)) / 255.0;
        float green = float(uint8_t(clearColor >> 16)) / 255.0;
        float blue = float(uint8_t(clearColor >> 8)) / 255.0;
        float alpha = float(uint8_t(clearColor)) / 255.0;
        glClearColor(red, green, blue, alpha);
    }

    if (depth) {
        flags |= GL_DEPTH_BUFFER_BIT;

        glClearDepth(depthValue);
    }

    if (stencil) {
        flags |= GL_STENCIL_BUFFER_BIT;

        glClearStencil(GLint(stencilValue));
    }

    if (flags)
        glClear(flags);
}

int RenderTarget::width()
{
    return mWidth;
}

int RenderTarget::height()
{
    return mHeight;
}

Gfx::Viewport* RenderTarget::addViewport(float x/* =0 */, float y/* =0 */, float w/* =1 */, float h/* =1 */, unsigned int zOrder/* =0 */)
{
    Viewport* vp = TD_NEW Viewport(this);

    vp->setPosition(Vector2(x, y), true);
    vp->setSize(Vector2(w, h), true);

    Viewports::value_type val(zOrder, vp);
    mViewports.insert(val);
    return vp;
}

void RenderTarget::releaseViewport(Gfx::Viewport* vp)
{
    for (Viewports::iterator it = mViewports.begin(); it != mViewports.end(); ++it) {
        if (it->second == vp) {
            mViewports.erase(it);
            delete vp;
            return;
        }
    }
}

} // namespace OpenGL
} // namespace Gfx
} // namespace Teardrop
