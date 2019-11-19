/******************************************************************************
Copyright (c) 2018 Teardrop Games

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

#include "Gfx/Common.h"
#include "Gfx/SynchronizationPrimitive.h"

#include <cstdint>
#include <memory>

namespace Teardrop {
namespace Gfx  {

class CommandBuffer;
class CommandQueue;
class RenderPass;
class Pipeline;
class RenderTarget;

class Renderer
{
public:
    virtual ~Renderer();

    // create a new render window; if hWnd is 0, creates a new top-level
    // window, otherwise, creates an embedded render context in the supplied
    // window;
    virtual std::shared_ptr<RenderTarget> createRenderWindow(uintptr_t hWnd, SurfaceFormat fmt, int flags) = 0;
    // create a new render texture; if tex is null, no render texture is created
    virtual std::shared_ptr<RenderTarget> createRenderTexture(int w, int h, SurfaceFormat fmt, int flags) = 0;
    virtual std::unique_ptr<CommandBuffer> createCommandBuffer(bool reusable) = 0;
    virtual std::unique_ptr<RenderPass> createRenderPass(char const* name = nullptr) = 0;
    virtual std::unique_ptr<Pipeline> createPipeline(PipelineType type, Gfx::RenderPass* renderPassTemplate) = 0;
    virtual std::unique_ptr<SynchronizationPrimitive> createSynchronizationPrimitive(SynchronizationPrimitiveType type, bool signaled) = 0;
    virtual Gfx::CommandQueue* getCommandQueue(size_t index) = 0;
    virtual size_t getCommandQueueCount() const = 0;
};

} // namespace Gfx
} // namespace Teardrop

