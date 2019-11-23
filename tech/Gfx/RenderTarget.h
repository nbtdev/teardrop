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

#include "Memory/Allocators.h"

#include <cstddef>

namespace Teardrop {
namespace Gfx  {

class CommandQueue;
struct SynchronizationPrimitive;
class Viewport;

class RenderTarget
{
public:
    RenderTarget();
    virtual ~RenderTarget();

    virtual void clear(
        bool color = true,
        unsigned int clearColor = 0,
        bool depth = true,
        float depthValue = 1,
        bool stencil = true,
        unsigned int stencilValue = 0) = 0;
    virtual float aspect() = 0;
    virtual int width() = 0;
    virtual int height() = 0;
    virtual void resize(int w, int h) = 0;
    virtual void setCurrent() = 0;
    virtual void unsetCurrent() = 0;
    virtual Viewport* addViewport(float x=0, float y=0, float w=1, float h=1, size_t zOrder=0) = 0;
    virtual size_t viewportCount(size_t zOrder = 0) const = 0;
    virtual Viewport* viewport(size_t index = 0, size_t zOrder = 0) const = 0;
    virtual void releaseViewport(Viewport* vp) = 0;
    virtual void presentQueue(CommandQueue* queue,
                              SynchronizationPrimitive* gpuWaitPrimitives, size_t gpuWaitCount,
                              SynchronizationPrimitive* cpuWaitPrimitive
                              ) = 0;
    virtual size_t swapchainLength() const = 0;
    virtual size_t currentSwapchainIndex() const = 0;
    virtual void waitForNext(Gfx::SynchronizationPrimitive* gpuWaitPrimitive,
                             Gfx::SynchronizationPrimitive* cpuWaitPrimitive) = 0;

    TD_DECLARE_ALLOCATOR();
};

} // namespace Gfx
} // namespace Teardrop
