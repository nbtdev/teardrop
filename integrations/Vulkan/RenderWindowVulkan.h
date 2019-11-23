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

#include "RenderTargetVulkan.h"

#include "Gfx/Common.h"

#include <vulkan/vulkan.h>

namespace Teardrop {
namespace Gfx {
namespace Vulkan {

class RenderWindow : public RenderTarget
{
public:
    RenderWindow(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device, uintptr_t hWnd, SurfaceFormat fmt, int flags);
    ~RenderWindow();

    // RenderTarget implementation
    void resize(int w, int h) override;
    void setCurrent() override;
    void unsetCurrent() override;
    void presentQueue(Gfx::CommandQueue* queue,
                      Gfx::SynchronizationPrimitive* gpuWaitPrimitives, size_t gpuWaitCount,
                      Gfx::SynchronizationPrimitive* cpuWaitPrimitive
                      ) override;
    size_t swapchainLength() const override;
    size_t currentSwapchainIndex() const override;
    void waitForNext(Gfx::SynchronizationPrimitive* gpuWaitPrimitive,
              Gfx::SynchronizationPrimitive* cpuWaitPrimitive) override;
    VkImage image() const override;
    VkFormat format() const override;
    VkImageView imageView() override;
    VkFramebuffer framebuffer(VkRenderPass renderPass) override;

    uint32_t frameCount() const;
    uint32_t imageCount() const;

    TD_DECLARE_ALLOCATOR();

private:
    int mInitFlags;
    VkSurfaceKHR mSurface;
    VkSwapchainKHR mSwapchain;
    VkPhysicalDevice mPhysicalDevice;
    VkInstance mInstance;

    VkFormat mImageFormat;
    VkImage* mImages;
    mutable VkImageView* mImageViews;
    mutable VkFramebuffer* mFramebuffers;
    uint32_t mImageCount;
    uint32_t mFrameCount;
    uint32_t mCurrentImageIndex;

    void createImageViews();
    void destroyImageViews();
    void createFrameBuffers(VkRenderPass renderPass);
    void destroyFrameBuffers();
};

} // namespace Vulkan
} // namespace Gfx
} // namespace Teardrop
