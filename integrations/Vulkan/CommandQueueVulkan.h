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

#include "Gfx/CommandQueue.h"

#include <vulkan/vulkan.h>

#include <vector>

namespace Teardrop {
namespace Gfx {
namespace Vulkan {

class CommandQueue : public Gfx::CommandQueue
{
public:
    class Submission : public Gfx::CommandQueue::Submission
    {
    public:
        Submission();
        ~Submission();

        // Gfx::CommandQueue::Submission implementation
        void addCommandBuffer(Gfx::CommandBuffer* commandBuffer) override;
        void addWaitPrimitive(SynchronizationPrimitive* primitive, uint32_t stageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT) override;
        void addSignalPrimitive(SynchronizationPrimitive* primitive) override;
        bool validate() const override;

        uint32_t commandBufferCount() const;
        VkCommandBuffer const* commandBuffers() const;
        uint32_t waitSemaphoreCount() const;
        VkSemaphore const* waitSemaphores() const;
        uint32_t signalSemaphoreCount() const;
        VkSemaphore const* signalSemaphores() const;
        uint32_t const* stageWaitMasks() const;

    private:
        std::vector<VkCommandBuffer> mCommandBuffers;
        std::vector<VkSemaphore> mWaitSemaphores;
        std::vector<VkSemaphore> mSignalSemaphores;
        std::vector<uint32_t> mStageWaitMasks;
    };

    CommandQueue(VkDevice device, VkQueue queue);
    ~CommandQueue();

    // Gfx::CommandQueue implementation
    void submit(Gfx::CommandQueue::Submission* submissionInfo, size_t submitCount, Gfx::SynchronizationPrimitive* cpuSignalPrimitive) override;

    VkQueue queue() const;

private:
    VkDevice mDevice;
    VkQueue mQueue;
};

} // namespace Vulkan
} // namespace Gfx
} // namespace Teardrop
