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

#if !defined(LANDSCAPEZONE_INCLUDED)
#define LANDSCAPEZONE_INCLUDED

#include "Asset/LandscapeAsset.h"
#include "Game/Scene.h"
#include "Math/Vector2.h"
#include "Math/Vector4.h"
#include "Memory/Allocators.h"

#include <memory>

namespace Teardrop {

class AABB;
class RenderContext;
class SceneRenderer;
class SceneRenderStep;

namespace Gfx {
    class Pipeline;
    class Renderable;
}

class LandscapeScene : public Scene
{
public:
    TD_CLASS(LandscapeScene, Scene);
    TD_CLASS_CREATABLE();

    TD_POINTER_PROPERTY(LandscapeAsset, "Landscape asset for this scene", LandscapeAsset, 0);
    TD_POINTER_PROPERTY(CameraController, "Camera controller", CameraController, 0);
    TD_COMPLEX_PROPERTY(Viewport, "Render viewport (integer/pixel dimensions)", Vector2, "(0,0)", 0);
    TD_COMPLEX_PROPERTY(Size, "Size, in world units, of landscape dimensions (Y is altitude), default: use heightfield dimensions", Vector4, "(0,0,0,0)", 0);

    //TD_POINTER_PROPERTY(HUD, "Heads-up display for this scene", HUD, 0);
    TD_PROPERTY(CastShadows, "Landscape casts shadows (default: yes)", bool, true, 0);
    TD_PROPERTY(ReceiveShadows, "Landscape receives shadows (default: yes)", bool, true, 0);
    TD_COMPLEX_PROPERTY(Tiling, "Number of tiles in X and Z ", Vector2, "(0,0)", 0);

    LandscapeScene();
    ~LandscapeScene();

    // Object overrides
    bool initialize() override;
    bool destroy() override;

    // Executable overrides
    bool onPreLoad() override;
    bool onPostLoad() override;
    void tick() override;
    void renderFrame(Gfx::Renderer* renderer, Gfx::RenderTarget* rt) override;
    bool onPreUnload() override;
    bool onPostUnload() override;
    void injectMouseMove(int absX, int absY, int relX = 0, int relY = 0) override;
    void injectMouseWheel(int absZ, int relZ = 0) override;

    TD_DECLARE_ALLOCATOR();

private:
    // TODO: this is a little strange at the outset; not sure the Executable should have this
    // stuff contained within it, expect to move this out during a later refactor
    SceneRenderer* mSceneRenderer;
    SceneRenderStep* mSceneRenderStep;
    std::unique_ptr<RenderContext> mRenderContext;

    Gfx::Renderable* mTerrainRenderables;
    Gfx::Pipeline* mTerrainPipeline;

    AABB* mBoundingBoxes;
    size_t mTerrainRenderableCount;

    void generateTerrainTiles();
};

} // namespace Teardrop

#endif // LANDSCAPEZONE_INCLUDED
