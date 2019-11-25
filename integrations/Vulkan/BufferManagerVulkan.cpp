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

#include "BufferManagerVulkan.h"
#include "IndexBufferVulkan.h"
#include "VertexBufferVulkan.h"

#include "Gfx/Submesh.h"

#include <algorithm>

namespace Teardrop {
namespace Gfx {
namespace Vulkan {

BufferManager::BufferManager(VkDevice device, VkPhysicalDevice physicalDevice)
    : mDevice(device)
    , mPhysicalDevice(physicalDevice)
{
}

BufferManager::~BufferManager()
{
    for (auto b : mIndexBuffers) {
        delete b;
    }

    for (auto b : mVertexBuffers) {
        delete b;
    }

//	for (auto v : mVertexDeclarations) {
//		delete v;
//	}
}

Gfx::IndexBuffer* BufferManager::createIndexBuffer(Submesh* parent)
{
    IndexBuffer* buf = TD_NEW IndexBuffer(parent, mDevice, mPhysicalDevice);
    mIndexBuffers.push_back(buf);
    return buf;
}

Gfx::VertexBuffer* BufferManager::createVertexBuffer(Submesh* parent)
{
    VertexBuffer* buf = TD_NEW VertexBuffer(parent, mDevice, mPhysicalDevice);
    mVertexBuffers.push_back(buf);
    return buf;
}

Gfx::VertexDeclaration* BufferManager::createVertexDeclaration(Submesh* parent)
{
    return nullptr;
}

void BufferManager::release(Gfx::IndexBuffer* buffer)
{
    mIndexBuffers.erase(std::find(mIndexBuffers.begin(), mIndexBuffers.end(), buffer));
    delete buffer;
}

void BufferManager::release(Gfx::VertexBuffer* buffer)
{
    mVertexBuffers.erase(std::find(mVertexBuffers.begin(), mVertexBuffers.end(), buffer));
    delete buffer;
}

void BufferManager::release(Gfx::VertexDeclaration* decl)
{

}

} // namespace Vulkan
} // namespace Gfx
} // namespace Teardrop
