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

#pragma once

#include "Gfx/RenderTarget.h"
#include "Memory/Allocators.h"

#include <vulkan/vulkan.h>

#include <map>

namespace Teardrop {
namespace Gfx {
namespace Vulkan {

class RenderTarget : public Gfx::RenderTarget
{
public:
    RenderTarget(VkDevice device);
    ~RenderTarget();

    void clear(
        bool color = true,
        unsigned int clearColor = 0,
        bool depth = true,
        float depthValue = 1,
        bool stencil = true,
        unsigned int stencilValue = 0) override;
    float aspect() override;
    int width() override;
    int height() override;
    Gfx::Viewport* addViewport(float x = 0, float y = 0, float w = 1, float h = 1, size_t zOrder = 0) override;
    size_t viewportCount(size_t zOrder) const override;
    Gfx::Viewport* viewport(size_t index = 0, size_t zOrder = 0) const override;
    void releaseViewport(Viewport* vp) override;

    VkFramebuffer framebuffer() const;

    TD_DECLARE_ALLOCATOR();

protected:
    int mWidth = 0;
    int mHeight = 0;

    typedef std::multimap<size_t /*zOrder*/, Viewport*> Viewports;
    Viewports mViewports;

    VkDevice mDevice;
    VkFramebuffer mFramebuffer;
};

} // namespace Vulkan
} // namespace Gfx
} // namespace Teardrop
