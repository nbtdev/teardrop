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

#include "RenderPassVulkan.h"

#include "AllocatorsVulkan.h"

namespace Teardrop {
namespace Gfx {
namespace Vulkan {

RenderPass::RenderPass(VkDevice device, char const* debugName)
    : Gfx::RenderPass(debugName)
    , mDevice(device)
    , mClearOnLoad(false)
    , mNeedsRebuild(false)
{
}

RenderPass::~RenderPass()
{
    if (mRenderPass != VK_NULL_HANDLE) {
        vkDestroyRenderPass(mDevice, mRenderPass, getAllocationCallbacks());
    }
}

void RenderPass::attachInputBuffer(IndexBuffer* buffer)
{
    mNeedsRebuild = true;
}

void RenderPass::attachInputBuffer(VertexBuffer* buffer)
{
    mNeedsRebuild = true;
}

void RenderPass::attachInputTexture(Texture* texture)
{
    mNeedsRebuild = true;
}

void RenderPass::attachOutput(Gfx::RenderTarget* renderTarget)
{
    mRenderTarget = (Vulkan::RenderTarget*)renderTarget;
    mNeedsRebuild = true;
}

void RenderPass::setClearColor(float r, float g, float b, float a)
{
    mClearOnLoad = true;
    mClearValue.float32[0] = r;
    mClearValue.float32[1] = g;
    mClearValue.float32[2] = b;
    mClearValue.float32[3] = a;
    mNeedsRebuild = true;
}

VkRenderPass RenderPass::renderPass()
{
    build();
    return mRenderPass;
}

void RenderPass::build()
{
    if (!mNeedsRebuild) {
        return;
    }

    if (mRenderPass != VK_NULL_HANDLE) {
        vkDestroyRenderPass(mDevice, mRenderPass, getAllocationCallbacks());
    }

    VkRenderPassCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    info.subpassCount = 0;

    vkCreateRenderPass(mDevice, &info, getAllocationCallbacks(), &mRenderPass);

    mNeedsRebuild = false;
}

} // namespace Vulkan
} // namespace Gfx
} // namespace Teardrop
