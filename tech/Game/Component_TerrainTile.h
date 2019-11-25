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

#include "Component_Render.h"

#include "Math/AABB.h"
#include "Memory/Allocators.h"

namespace Teardrop {

namespace Gfx {

class Pipeline;
class Renderable;

} // namespace Gfx

class LandscapeScene;

class TerrainTileComponent : public RenderComponent
{
    TD_CLASS(TerrainTileComponent, RenderComponent);

    void initializeTile(int tileX, int tileY, LandscapeScene* scene);
    void setPipeline(Gfx::Pipeline* pipeline);

    AABB const& boundingBox() const;
    Gfx::Renderable const& renderable() const;

    TD_DECLARE_ALLOCATOR();

protected:
    AABB mAABB;
};

} // namespace Teardrop
