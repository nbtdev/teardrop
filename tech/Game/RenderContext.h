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

#include "Memory/Allocators.h"

#include <memory>
#include <vector>

namespace Teardrop {

namespace Gfx {
class Camera;
class CommandQueue;
class Renderer;
class RenderTarget;
class SynchronizationPrimitive;
} // namespace Gfx

class Context
{
public:
    virtual ~Context();

    virtual Gfx::Renderer* renderer() const = 0;
    virtual Gfx::RenderTarget* renderTarget() const = 0;
    virtual Gfx::Camera* camera() const = 0;
    virtual Gfx::CommandQueue* graphicsQueue() const = 0;
    virtual Gfx::CommandQueue* computeQueue() const = 0;
    virtual Gfx::CommandQueue* blitQueue() const = 0;
    virtual Gfx::CommandQueue* presentQueue() const = 0;

    TD_DECLARE_ALLOCATOR();
};

class RenderContext : public Context
{
public:
    RenderContext(Gfx::Renderer* renderer, Gfx::RenderTarget* renderTarget);
    ~RenderContext();

    // Context implementation
    Gfx::Renderer* renderer() const override;
    Gfx::RenderTarget* renderTarget() const override;
    Gfx::Camera* camera() const override;
    Gfx::CommandQueue* graphicsQueue() const override;
    Gfx::CommandQueue* computeQueue() const override;
    Gfx::CommandQueue* blitQueue() const override;
    Gfx::CommandQueue* presentQueue() const override;

    void beginFrame(Gfx::Camera* camera);
    void endFrame();

    TD_DECLARE_ALLOCATOR();

private:
    Gfx::Renderer* mRenderer;
    Gfx::RenderTarget* mRenderTarget;
    Gfx::Camera* mCamera;
    size_t mFrameNumber;

    // semaphores to signal when the GPU is done rendering to the render target
    std::vector<std::unique_ptr<Gfx::SynchronizationPrimitive>> mRenderTargetFinishedPrimitives;
    // fence to notify when presentation system is finished with the render target
    std::vector<std::unique_ptr<Gfx::SynchronizationPrimitive>> mFramePresentFinishedPrimitives;
};

} // namespace Teardrop
