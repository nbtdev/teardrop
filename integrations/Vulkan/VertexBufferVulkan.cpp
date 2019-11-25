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

#include "VertexBufferVulkan.h"

#include "AllocatorsVulkan.h"

#include <cassert>

namespace {

uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t filter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProps = {};
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProps);

    for (uint32_t i=0; i<memProps.memoryTypeCount; ++i) {
        if (filter & (1 << i)) {
            if ((memProps.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }
    }

    return UINT32_MAX;
}

} // namespace

namespace Teardrop {
namespace Gfx {
namespace Vulkan {

VertexBuffer::VertexBuffer(Submesh* parent, VkDevice device, VkPhysicalDevice physicalDevice)
    : Gfx::VertexBuffer(parent)
    , mDevice(device)
    , mPhysicalDevice(physicalDevice)
    , mMemory(VK_NULL_HANDLE)
    , mBuffer(VK_NULL_HANDLE)
{

}

VertexBuffer::~VertexBuffer()
{
    if (mBuffer != VK_NULL_HANDLE) {
        vkDestroyBuffer(mDevice, mBuffer, getAllocationCallbacks());
    }

    if (mMemory != VK_NULL_HANDLE) {
        vkFreeMemory(mDevice, mMemory, getAllocationCallbacks());
    }
}

void VertexBuffer::initialize(size_t vertexCount, size_t initFlags, void* data)
{
    VkDeviceSize bufferSize = (VkDeviceSize)(vertexCount * mSize);
    mCount = vertexCount;

    // create the buffer object
    VkBufferCreateInfo bufferCreateInfo = {};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferCreateInfo.size = bufferSize;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    VkResult r = vkCreateBuffer(mDevice, &bufferCreateInfo, getAllocationCallbacks(), &mBuffer);
    if (VK_SUCCESS != r) {
        return;
    }

    // create the memory to store its data
    VkMemoryRequirements reqs = {};
    vkGetBufferMemoryRequirements(mDevice, mBuffer, &reqs);

    uint32_t memoryTypeIndex = findMemoryType(mPhysicalDevice, reqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT|VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    if (memoryTypeIndex == UINT32_MAX) {
        return;
    }

    VkMemoryAllocateInfo memoryAllocateInfo = {};
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.allocationSize  = reqs.size;
    memoryAllocateInfo.memoryTypeIndex = memoryTypeIndex;
    r = vkAllocateMemory(mDevice, &memoryAllocateInfo, getAllocationCallbacks(), &mMemory);
    if (VK_SUCCESS != r) {
        return;
    }

    // bind the buffer object to the memory
    vkBindBufferMemory(mDevice, mBuffer, mMemory, 0);}

void VertexBuffer::resize(size_t vertexCount)
{
    assert(false && "Not implemented");
}

void* VertexBuffer::map(MapFlags flags)
{
    if (mBuffer == VK_NULL_HANDLE) {
        return nullptr;
    }

    void* rtn = nullptr;
    VkDeviceSize size = (VkDeviceSize)mCount * (VkDeviceSize)mSize;
    vkMapMemory(mDevice, mMemory, 0, size, 0, &rtn);
    return rtn;
}

void VertexBuffer::unmap()
{
    if (mBuffer == VK_NULL_HANDLE) {
        return;
    }

    vkUnmapMemory(mDevice, mMemory);
}

VkBuffer VertexBuffer::buffer() const
{
    return mBuffer;
}

} // namespace Vulkan
} // namespace Gfx
} // namespace Teardrop
