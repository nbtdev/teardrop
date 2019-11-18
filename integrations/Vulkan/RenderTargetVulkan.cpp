/******************************************************************************
Copyright (c) 2019 Teardrop Games

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

#include "RenderTargetVulkan.h"

#include "AllocatorsVulkan.h"
#include "ViewportVulkan.h"

#include "Math/Vector2.h"

#include <cassert>
#include <cstdint>

namespace Teardrop {
namespace Gfx {
namespace Vulkan {

RenderTarget::RenderTarget(VkDevice device)
    : mDevice(device)
    , mFramebuffer(VK_NULL_HANDLE)
{
}

RenderTarget::~RenderTarget()
{
    for (Viewports::iterator it = mViewports.begin(); it != mViewports.end(); ++it) {
        delete it->second;
    }

    if (mFramebuffer != VK_NULL_HANDLE) {
        vkDestroyFramebuffer(mDevice, mFramebuffer, getAllocationCallbacks());
    }
}

void RenderTarget::clear(
    bool color /* = true */, unsigned int clearColor /* = 0 */,
    bool depth /* = true */, float depthValue /* = 1 */,
    bool stencil /* = true */, unsigned int stencilValue /* = 0 */)
{
}

float RenderTarget::aspect()
{
    return float(mWidth) / float(mHeight);
}

int RenderTarget::width()
{
    return mWidth;
}

int RenderTarget::height()
{
    return mHeight;
}

Gfx::Viewport* RenderTarget::addViewport(float x/* =0 */, float y/* =0 */, float w/* =1 */, float h/* =1 */, size_t zOrder/* =0 */)
{
    Viewport* vp = TD_NEW Viewport(this, mDevice);

    vp->setPosition(Vector2(x, y), true);
    vp->setSize(Vector2(w, h), true);

    Viewports::value_type val(zOrder, vp);
    mViewports.insert(val);
    return vp;
}

size_t RenderTarget::viewportCount(size_t zOrder) const
{
    size_t count = (size_t)mViewports.count(zOrder);
    return count;
}

Gfx::Viewport* RenderTarget::viewport(size_t index, size_t zOrder) const
{
    size_t count = viewportCount(zOrder);

    if (index >= count) {
        return nullptr;
    }

    auto it = mViewports.lower_bound(zOrder);
    while (index--) {
        ++it;
    }

    return it->second;
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

VkFramebuffer RenderTarget::framebuffer() const
{
    return mFramebuffer;
}

} // namespace Vulkan
} // namespace Gfx
} // namespace Teardrop
