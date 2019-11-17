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

#include "SynchronizationPrimitiveVulkan.h"

#include "AllocatorsVulkan.h"

namespace Teardrop {
namespace Gfx {
namespace Vulkan {

SynchronizationPrimitive::SynchronizationPrimitive(SynchronizationPrimitiveType type, bool signaled, VkDevice device)
    : mDevice(device)
{
    mType = type;
    mPrimitive.event = VK_NULL_HANDLE;

    if (type == SYNCH_TYPE_GPU_TO_GPU) {
        VkSemaphoreCreateInfo info = {
            VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            nullptr,
            0
        };

        vkCreateSemaphore(device, &info, getAllocationCallbacks(), &mPrimitive.semaphore);
    }

    if (type == SYNCH_TYPE_CPU_TO_GPU) {
        VkEventCreateInfo info = {
            VK_STRUCTURE_TYPE_EVENT_CREATE_INFO,
            nullptr,
            0
        };

        vkCreateEvent(device, &info, getAllocationCallbacks(), &mPrimitive.event);
    }

    if (type == SYNCH_TYPE_GPU_TO_CPU) {
        uint32_t flags = 0;

        if (signaled) {
            flags |= VK_FENCE_CREATE_SIGNALED_BIT;
        }

        VkFenceCreateInfo info = {
            VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            nullptr,
            flags
        };

        vkCreateFence(device, &info, getAllocationCallbacks(), &mPrimitive.fence);
    }
}

SynchronizationPrimitive::~SynchronizationPrimitive()
{
    switch(mType) {
    case SYNCH_TYPE_CPU_TO_GPU:
        vkDestroyEvent(mDevice, mPrimitive.event, getAllocationCallbacks());
        break;
    case SYNCH_TYPE_GPU_TO_GPU:
        vkDestroySemaphore(mDevice, mPrimitive.semaphore, getAllocationCallbacks());
        break;
    case SYNCH_TYPE_GPU_TO_CPU:
        vkDestroyFence(mDevice, mPrimitive.fence, getAllocationCallbacks());
        break;
    }
}

} // namespace Vulkan
} // namespace Gfx
} // namespace Teardrop
