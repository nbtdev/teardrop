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

#include "Game/RenderContext.h"

#include "Gfx/Renderer.h"
#include "Gfx/RenderTarget.h"

#include <cassert>

namespace Teardrop {

Context::~Context()
{
}

RenderContext::RenderContext(Gfx::Renderer* renderer, Gfx::RenderTarget* renderTarget)
    : mRenderer(renderer)
    , mRenderTarget(renderTarget)
    , mCamera(nullptr)
    , mFrameNumber(0)
{
    assert(mRenderer);
    assert(mRenderTarget);

    if (!mRenderer && !mRenderTarget) {
        return;
    }

    // initialize the synch primitives that orchestrate the access to render targets
    size_t swapchainLength = mRenderTarget->swapchainLength();

    for (size_t i=0; i<swapchainLength; ++i) {
        mRenderTargetFinishedPrimitives.push_back(mRenderer->createSynchronizationPrimitive(Gfx::SYNCH_TYPE_GPU_TO_GPU, true));
        mFramePresentFinishedPrimitives.push_back(mRenderer->createSynchronizationPrimitive(Gfx::SYNCH_TYPE_GPU_TO_CPU, true));
    }
}

RenderContext::~RenderContext()
{
}

Gfx::Renderer* RenderContext::renderer() const
{
    return mRenderer;
}

Gfx::RenderTarget* RenderContext::renderTarget() const
{
    return mRenderTarget;
}

Gfx::Camera* RenderContext::camera() const
{
    return mCamera;
}

Gfx::SynchronizationPrimitive* RenderContext::framePresentFinishedPrimitive() const
{
    return mFramePresentFinishedPrimitives[mRenderTarget->currentSwapchainIndex()].get();
}

Gfx::CommandQueue* RenderContext::graphicsQueue() const
{
    assert(mRenderer);
    if (!mRenderer) {
        return nullptr;
    }

    // for now, single (first) queue is the graphics queue
    return mRenderer->getCommandQueue(0);
}

Gfx::CommandQueue* RenderContext::computeQueue() const
{
    assert(mRenderer);
    if (!mRenderer) {
        return nullptr;
    }

    // for now, single (first) queue is also the compute queue
    // TODO: make sure that this queue can do compute
    return mRenderer->getCommandQueue(0);
}

Gfx::CommandQueue* RenderContext::blitQueue() const
{
    assert(mRenderer);
    if (!mRenderer) {
        return nullptr;
    }

    // for now, single (first) queue is also the blit queue
    // TODO: make sure that this queue can do blit
    return mRenderer->getCommandQueue(0);
}

Gfx::CommandQueue* RenderContext::presentQueue() const
{
    assert(mRenderer);
    if (!mRenderer) {
        return nullptr;
    }

    // for now, we don't do a separate present queue
    return nullptr;
}

void RenderContext::beginFrame(Gfx::Camera* camera)
{
    mCamera = camera;

    // obtain exclusive access to an available target in the swapchain
    size_t index = mFrameNumber % mRenderTargetFinishedPrimitives.size();
    mRenderTarget->waitForNext(mRenderTargetFinishedPrimitives[index].get(), mFramePresentFinishedPrimitives[index].get());
}

void RenderContext::endFrame()
{
    size_t index = mFrameNumber % mRenderTargetFinishedPrimitives.size();
    mRenderTarget->presentQueue(graphicsQueue(), mRenderTargetFinishedPrimitives[index].get(), 1, mFramePresentFinishedPrimitives[index].get());
    mFrameNumber++;
}

} // namespace Teardrop
