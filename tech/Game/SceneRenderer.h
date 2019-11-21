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

#include <memory>
#include <vector>

namespace Teardrop {

namespace Gfx {
class Camera;
} // namespace Gfx

class ZoneObject;
class Context;
class RenderStep;
class Scene;

/*
    Class to manage multiple steps in a single-frame render.
    Note that the render call takes the "main scene cam" as
    a parameter; this is an optimization to allow this class
    to cache the list of objects visible with this camera, in
    anticipation of reuse of same by multiple different
    RenderStep implementations. At the least, it will need to
    be done once per frame anyway. All RenderStep::render()
    implementations take this list as a parameter, but they are
    not required to use it, of course.
*/

class SceneRenderer
{
public:
    SceneRenderer();
    virtual ~SceneRenderer();

    // when you add a step, this class will take ownership of it and clean it up on destruction;
    // RenderSteps are executed in the order in which they are added.
    virtual void addStep(RenderStep* pStep);

    typedef std::vector<ZoneObject*> ZoneObjects;
    virtual void render(
        const ZoneObjects& visbleObjects,	// list of objects visible through the camera
        Context* context,
        Scene* pScene
        );

    TD_DECLARE_ALLOCATOR();

private:
    typedef std::vector<std::unique_ptr<RenderStep>> RenderSteps;
    RenderSteps mRenderSteps;
};

} // namespace Teardrop
