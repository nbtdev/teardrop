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

#include "CommandBufferVulkan.h"

#include "IndexBufferVulkan.h"
#include "PipelineVulkan.h"
#include "RenderPassVulkan.h"
#include "RenderTargetVulkan.h"
#include "VertexBufferVulkan.h"
#include "ViewportVulkan.h"

#include <cassert>

namespace Teardrop {
namespace Gfx {
namespace Vulkan {

CommandBuffer::CommandBuffer(VkDevice device, VkCommandBuffer commandBuffer, VkCommandPool pool, bool reusable)
    : mDevice(device)
    , mCommandBuffer(commandBuffer)
    , mPool(pool)
    , mReusable(reusable)
{
    assert(commandBuffer != VK_NULL_HANDLE);
}

CommandBuffer::~CommandBuffer()
{
    if (VK_NULL_HANDLE != mCommandBuffer) {
        vkFreeCommandBuffers(mDevice, mPool, 1, &mCommandBuffer);
    }
}

void CommandBuffer::beginRecording()
{
    uint32_t flags = 0;
    if (mReusable) {
        flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    }

    VkCommandBufferBeginInfo info = {
        VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        nullptr,
        flags,
        nullptr // TODO: support secondary command buffers
    };

    vkBeginCommandBuffer(mCommandBuffer, &info);
}

void CommandBuffer::endRecording()
{
    vkEndCommandBuffer(mCommandBuffer);
}

void CommandBuffer::reset()
{
    vkResetCommandBuffer(mCommandBuffer, 0);
}

void CommandBuffer::beginRenderPass(Gfx::RenderPass* renderPass, Gfx::RenderTarget* renderTarget)
{
    assert(renderPass);
    if (!renderPass) {
        return;
    }

    Vulkan::RenderPass* pass = (Vulkan::RenderPass*)renderPass;
    Vulkan::RenderTarget* rt = (Vulkan::RenderTarget*)renderTarget;

    VkClearValue clearValue = {};
    clearValue.color = pass->clearColorValue();

    VkRenderPassBeginInfo info = {
        VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        nullptr,
        pass->renderPass(),
        rt->framebuffer(pass->renderPass()),
        {
            {0, 0},
            {(uint32_t)rt->width(), (uint32_t)rt->height()}
        },
        1,
        &clearValue
    };

    vkCmdBeginRenderPass(mCommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
}

void CommandBuffer::endRenderPass()
{
    vkCmdEndRenderPass(mCommandBuffer);
}

void CommandBuffer::setViewport(Gfx::Viewport* vp)
{
    assert(vp);
    if (!vp) {
        return;
    }

    Vulkan::Viewport* vvp = (Vulkan::Viewport*)vp;
    VkViewport const& viewport = vvp->viewport();
    vkCmdSetViewport(mCommandBuffer, 0, 1, &viewport);
}

void CommandBuffer::bindPipeline(Gfx::Pipeline* pipeline)
{
    assert(pipeline);
    if (!pipeline) {
        return;
    }

    Vulkan::Pipeline* vulkanPipeline = (Vulkan::Pipeline*)pipeline;
    vkCmdBindPipeline(mCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline->pipeline());
}

void CommandBuffer::bindIndexBuffer(Gfx::IndexBuffer* buffer)
{
    assert(buffer);
    if (!buffer) {
        return;
    }

    Vulkan::IndexBuffer* vulkanBuffer = (Vulkan::IndexBuffer*)buffer;

    VkIndexType indexType = VK_INDEX_TYPE_MAX_ENUM;
    if (vulkanBuffer->indexSize() == 2) {
        indexType = VK_INDEX_TYPE_UINT16;
    } else if (vulkanBuffer->indexSize() == 4) {
        indexType = VK_INDEX_TYPE_UINT32;
    }

    if (indexType == VK_INDEX_TYPE_MAX_ENUM) {
        return;
    }

    vkCmdBindIndexBuffer(mCommandBuffer, vulkanBuffer->buffer(), 0, indexType);
}

void CommandBuffer::bindVertexBuffers(Gfx::VertexBuffer** buffers, size_t bufferCount)
{
    assert(buffers);
    if (!buffers || !bufferCount) {
        return;
    }

    VkBuffer* vertexBuffers = (VkBuffer*)alloca(sizeof(VkBuffer) * bufferCount);
    VkDeviceSize* offsets = (VkDeviceSize*)alloca(sizeof(VkDeviceSize) * bufferCount);

    Vulkan::VertexBuffer** vBuffers = (Vulkan::VertexBuffer**)buffers;
    for (uint32_t i=0; i<bufferCount; ++i) {
        vertexBuffers[i] = vBuffers[i]->buffer();
        offsets[i] = 0;
    }

    vkCmdBindVertexBuffers(mCommandBuffer, 0, (uint32_t)bufferCount, vertexBuffers, offsets);
}

void CommandBuffer::bindDescriptorSets(DescriptorSet** sets, size_t setCount)
{

}

void CommandBuffer::draw(size_t vertexCount, size_t startingVertex)
{
    vkCmdDraw(mCommandBuffer, (uint32_t)vertexCount, 0, (uint32_t)startingVertex, 0);
}

void CommandBuffer::drawIndexed(size_t indexCount, size_t startingIndex)
{
    vkCmdDrawIndexed(mCommandBuffer, (uint32_t)indexCount, 0, (uint32_t)startingIndex, 0, 0);
}

void CommandBuffer::drawInstanced(size_t vertexCount, size_t startingVertex, size_t instanceCount, size_t startingInstance)
{
    vkCmdDraw(mCommandBuffer, (uint32_t)vertexCount, (uint32_t)instanceCount, (uint32_t)startingVertex, (uint32_t)startingInstance);
}

void CommandBuffer::drawInstancedIndexed(size_t indexCount, size_t startingIndex, size_t instanceCount, size_t startingInstance)
{
    vkCmdDrawIndexed(mCommandBuffer, (uint32_t)indexCount, (uint32_t)instanceCount, (uint32_t)startingIndex, 0, (uint32_t)startingIndex);
}

VkCommandBuffer CommandBuffer::commandBuffer() const
{
    return mCommandBuffer;
}


} // namespace Vulkan
} // namespace Gfx
} // namespace Teardrop
