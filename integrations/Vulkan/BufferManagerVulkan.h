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

#include "Gfx/BufferManager.h"

#include <vulkan/vulkan.h>

#include <vector>

namespace Teardrop {
namespace Gfx {
namespace Vulkan {

class IndexBuffer;
class VertexBuffer;
class VertexDeclaration;

class BufferManager : public Gfx::BufferManager
{
public:
    BufferManager(VkDevice device, VkPhysicalDevice physicalDevice);
    ~BufferManager();

    // Gfx::BufferManager implementation
    Gfx::IndexBuffer* createIndexBuffer(Submesh* parent) override;
    Gfx::VertexBuffer* createVertexBuffer(Submesh* parent) override;
    Gfx::VertexDeclaration* createVertexDeclaration(Submesh* parent) override;
    void release(Gfx::IndexBuffer* buffer) override;
    void release(Gfx::VertexBuffer* buffer) override;
    void release(Gfx::VertexDeclaration* decl) override;

    TD_DECLARE_ALLOCATOR();

protected:
    typedef std::vector<IndexBuffer*> IndexBuffers;
    typedef std::vector<VertexBuffer*> VertexBuffers;
    typedef std::vector<VertexDeclaration*> VertexDeclarations;
    IndexBuffers mIndexBuffers;
    VertexBuffers mVertexBuffers;
    VertexDeclarations mVertexDeclarations;

    VkDevice mDevice;
    VkPhysicalDevice mPhysicalDevice;
};

} // namespace Vulkan
} // namespace Gfx
} // namespace Teardrop
