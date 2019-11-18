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

#include "Gfx/CommandBuffer.h"

#include <vulkan/vulkan.h>

namespace Teardrop {
namespace Gfx {
namespace Vulkan {

class CommandBuffer : public Gfx::CommandBuffer
{
public:
    CommandBuffer(VkDevice device, VkCommandBuffer commandBuffer, bool reusable);
    ~CommandBuffer();

    // Gfx::CommandBuffer implementation
    void beginRecording() override;
    void endRecording() override;
    void reset() override;
    void beginRenderPass(RenderPass* renderPass, RenderTarget* renderTarget, Pipeline* pipeline) override;
    void endRenderPass() override;
    void setViewport(Viewport* vp) override;
    void bindIndexBuffer(IndexBuffer* buffer) override;
    void bindVertexBuffers(VertexBuffer** buffer, size_t bufferCount) override;
    void bindDescriptorSets(DescriptorSet** sets, size_t setCount) override;
    void draw(size_t vertexCount, size_t startingVertex) override;
    void drawIndexed(size_t indexCount, size_t startingIndex) override;
    void drawInstanced(size_t vertexCount, size_t startingVertex, size_t instanceCount, size_t startingInstance) override;
    void drawInstancedIndexed(size_t indexCount, size_t startingIndex, size_t instanceCount, size_t startingInstance) override;

    VkCommandBuffer commandBuffer() const;

private:
    VkDevice mDevice;
    VkCommandBuffer mCommandBuffer;
    bool mReusable;
};

} // namespace Vulkan
} // namespace Gfx
} // namespace Teardrop
