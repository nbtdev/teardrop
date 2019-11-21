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

#include <Memory/Allocators.h>

#include <cstddef>

namespace Teardrop {
namespace Gfx {

class DescriptorSet;
class IndexBuffer;
class VertexBuffer;
class Pipeline;
class RenderPass;
class RenderTarget;
class Viewport;

class CommandBuffer
{
public:
    virtual ~CommandBuffer();

    virtual void beginRecording() = 0;
    virtual void endRecording() = 0;
    virtual void reset() = 0;

    virtual void beginRenderPass(RenderPass* renderPass, RenderTarget* renderTarget) = 0;
    virtual void endRenderPass() = 0;
    virtual void setViewport(Viewport* vp) = 0;
    virtual void bindPipeline(Pipeline* pipeline) = 0;
    virtual void bindIndexBuffer(IndexBuffer* buffer) = 0;
    virtual void bindVertexBuffers(VertexBuffer** buffer, size_t bufferCount) = 0;
    virtual void bindDescriptorSets(DescriptorSet** sets, size_t setCount) = 0;
    virtual void draw(size_t vertexCount, size_t startingVertex) = 0;
    virtual void drawIndexed(size_t indexCount, size_t startingIndex) = 0;
    virtual void drawInstanced(size_t vertexCount, size_t startingVertex, size_t instanceCount, size_t startingInstance) = 0;
    virtual void drawInstancedIndexed(size_t indexCount, size_t startingIndex, size_t instanceCount, size_t startingInstance) = 0;

    TD_DECLARE_ALLOCATOR();
};

} // namespace Gfx
} // namespace Teardrop
