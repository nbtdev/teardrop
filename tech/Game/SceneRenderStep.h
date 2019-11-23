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

#include "RenderStep.h"

#include <memory>
#include <vector>

namespace Teardrop {

namespace Gfx {
class CommandBuffer;
class Viewport;
} // namespace Gfx

class Scene;

/**
 * @brief Render step that renders the normal 3D perspective view of the scene
 */
class SceneRenderStep : public RenderStep
{
public:
    SceneRenderStep();
    ~SceneRenderStep();

    void setViewport(Gfx::Viewport* pVP) { mVP = pVP; }

    // RenderStep implementation
    void render(const VisibleObjects& objects, Context* context, Scene* pScene) override;

    TD_DECLARE_ALLOCATOR();

private:
    Gfx::Viewport* mVP;

    std::vector<std::unique_ptr<Gfx::CommandBuffer>> mCommandBuffers;
};

} // namespace Teardrop
