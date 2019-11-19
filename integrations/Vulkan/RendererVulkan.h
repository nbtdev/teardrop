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

#include "Gfx/Renderer.h"

#include "Memory/Allocators.h"

#include <vulkan/vulkan.h>

#include <map>
#include <memory>
#include <vector>

namespace Teardrop {
namespace Gfx {
namespace Vulkan {

class Renderer : public Gfx::Renderer
{
public:
    Renderer(int flags);
    ~Renderer();

    // Gfx::Renderer implementation
    std::shared_ptr<Gfx::RenderTarget> createRenderWindow(uintptr_t hWnd, SurfaceFormat fmt, int flags) override;
    std::shared_ptr<Gfx::RenderTarget> createRenderTexture(int w, int h, SurfaceFormat fmt, int flags) override;
    std::unique_ptr<CommandBuffer> createCommandBuffer(bool reusable) override;
    std::unique_ptr<RenderPass> createRenderPass() override;
    std::unique_ptr<Pipeline> createPipeline(PipelineType type) override;
    CommandQueue* getCommandQueue(size_t index) override;
    SynchronizationPrimitive* createSynchronizationPrimitive(SynchronizationPrimitiveType type, bool signaled) override;
    size_t getCommandQueueCount() const override;

    TD_DECLARE_ALLOCATOR();

private:
    typedef std::vector<std::shared_ptr<Gfx::RenderTarget>> RenderTargets;
    RenderTargets mRenderTargets;

    VkInstance mInstance;
    VkPhysicalDevice mPhysicalDevice;
    VkDevice mDevice;
    VkCommandPool mTransientCommandPool;
    VkCommandPool mResetCommandPool;
};

} // namespace Vulkan
} // namespace Gfx
} // namespace Teardrop
