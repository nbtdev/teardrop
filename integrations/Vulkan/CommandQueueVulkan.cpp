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

#include "CommandQueueVulkan.h"

#include "CommandBufferVulkan.h"
#include "SynchronizationPrimitiveVulkan.h"

#include <alloca.h>
#include <vulkan/vulkan.h>

#include <cassert>

namespace Teardrop {
namespace Gfx {
namespace Vulkan {

Submission::Submission()
{
}

Submission::~Submission()
{
}

void Submission::addCommandBuffer(Gfx::CommandBuffer* commandBuffer)
{
    assert(commandBuffer);
    if (!commandBuffer) {
        return;
    }

    Vulkan::CommandBuffer* cmdBuf = (Vulkan::CommandBuffer*)commandBuffer;
    mCommandBuffers.push_back(cmdBuf->commandBuffer());
}

void Submission::addWaitPrimitive(Gfx::SynchronizationPrimitive* primitive, uint32_t stageMask)
{
    assert(primitive);
    if (!primitive) {
        return;
    }

    Vulkan::SynchronizationPrimitive* prim = (Vulkan::SynchronizationPrimitive*)prim;
    mWaitSemaphores.push_back(prim->mPrimitive.semaphore);
    mStageWaitMasks.push_back(stageMask);
}

void Submission::addSignalPrimitive(Gfx::SynchronizationPrimitive* primitive)
{
    assert(primitive);
    if (!primitive) {
        return;
    }

    Vulkan::SynchronizationPrimitive* prim = (Vulkan::SynchronizationPrimitive*)prim;
    mSignalSemaphores.push_back(prim->mPrimitive.semaphore);
}

void Submission::clear()
{
    mCommandBuffers.clear();
    mWaitSemaphores.clear();
    mSignalSemaphores.clear();
    mStageWaitMasks.clear();
}

bool Submission::validate() const
{
    return true;
}

uint32_t Submission::commandBufferCount() const
{
    return (uint32_t)mCommandBuffers.size();
}

VkCommandBuffer const* Submission::commandBuffers() const
{
    return mCommandBuffers.data();
}

uint32_t Submission::waitSemaphoreCount() const
{
    return (uint32_t)mWaitSemaphores.size();
}

VkSemaphore const* Submission::waitSemaphores() const
{
    return mWaitSemaphores.data();
}

uint32_t Submission::signalSemaphoreCount() const
{
    return (uint32_t)mSignalSemaphores.size();
}

VkSemaphore const* Submission::signalSemaphores() const
{
    return mSignalSemaphores.data();
}

uint32_t const* Submission::stageWaitMasks() const
{
    return mStageWaitMasks.data();
}

CommandQueue::CommandQueue(VkDevice device, VkQueue queue)
    : mDevice(device)
    , mQueue(queue)
{
}

CommandQueue::~CommandQueue()
{
}

std::unique_ptr<Gfx::CommandQueueSubmission> CommandQueue::createSubmission()
{
    return std::unique_ptr<Gfx::CommandQueueSubmission>(new Vulkan::Submission);
}

void CommandQueue::submit(Gfx::CommandQueueSubmission* submissionInfo, size_t submitCount, Gfx::SynchronizationPrimitive* cpuSignalPrimitive)
{
    assert(submissionInfo);
    if (!submissionInfo) {
        return;
    }

    Vulkan::Submission* vkSubmissions = (Vulkan::Submission*)submissionInfo;

    VkFence fence = VK_NULL_HANDLE;
    if (cpuSignalPrimitive) {
        Vulkan::SynchronizationPrimitive* fencePrim = (Vulkan::SynchronizationPrimitive*)cpuSignalPrimitive;
        fence = fencePrim->mPrimitive.fence;
    }

    VkSubmitInfo* submitInfos = (VkSubmitInfo*)alloca(sizeof(VkSubmitInfo) * submitCount);

    for (uint32_t i=0; i<submitCount; ++i) {
        submitInfos[i] = {};
        submitInfos[i].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfos[i].pCommandBuffers = vkSubmissions[i].commandBuffers();
        submitInfos[i].commandBufferCount = vkSubmissions[i].commandBufferCount();
        submitInfos[i].pWaitSemaphores = vkSubmissions[i].waitSemaphores();
        submitInfos[i].waitSemaphoreCount = vkSubmissions[i].waitSemaphoreCount();
        submitInfos[i].pSignalSemaphores = vkSubmissions[i].signalSemaphores();
        submitInfos[i].signalSemaphoreCount = vkSubmissions[i].signalSemaphoreCount();
        submitInfos[i].pWaitDstStageMask = vkSubmissions[i].stageWaitMasks();
    }

    vkQueueSubmit(mQueue, (uint32_t)submitCount, submitInfos, fence);
}

VkQueue CommandQueue::queue() const
{
    return mQueue;
}


} // namespace Vulkan
} // namespace Gfx
} // namespace Teardrop
