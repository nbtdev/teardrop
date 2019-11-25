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

#include "LandscapeScene.h"

#include "Game/Component_TerrainTile.h"
#include "Game/RenderContext.h"
#include "Game/SceneRenderer.h"
#include "Game/SceneRenderStep.h"
#include "Game/TerrainPatch.h"
#include "Game/Zone.h"
#include "Game/ZoneObjects.h"
#include "Gfx/Camera.h"
#include "Gfx/Connection.h"
#include "Gfx/Mesh.h"
#include "Gfx/Pipeline.h"
#include "Gfx/RenderTarget.h"
#include "Gfx/Sampler2DExpression.h"
#include "Gfx/Submesh.h"
#include "Gfx/VertexBuffer.h"
#include "Util/Hash.h"

namespace {

using namespace Teardrop;

// terrain material has only to deal with a single submesh, so we only need one material
Gfx::Pipeline* createTerrainPipeline(LandscapeAsset* landscapeAsset, Gfx::VertexBuffer const* vertexBuffer)
{
    TextureAsset* colorAsset = landscapeAsset->getDiffuseMap();

    // to begin, we need to make a material...
    Gfx::Pipeline* pipeline = TD_NEW Gfx::Pipeline;
    Gfx::Material* mtl = pipeline->material();
    mtl->initialize();

    // then, need an output expression for the material
    Gfx::MaterialOutput* output = TD_NEW Gfx::MaterialOutput;
    UUID uuid;
    uuid.generate();
    output->setObjectId(uuid);
    output->initialize();
    mtl->setOutput(output);

    // hook up the color (diffuse) texture/sampler to the output
    if (colorAsset) {
        Gfx::Sampler2DExpression* expr = TD_NEW Gfx::Sampler2DExpression;
        uuid.generate();
        expr->setObjectId(uuid);
        expr->initialize();
        expr->getSampler2D().setTextureAsset(colorAsset);

        Gfx::Connection* conn = TD_NEW Gfx::Connection;
        uuid.generate();
        conn->setFromExpression(expr); conn->setFromAttribute("Color");
        conn->setToExpression(output); conn->setToAttribute("Diffuse");
        conn->setParent(mtl);
        conn->setObjectId(uuid);
        conn->initialize();
    }

    // TODO: hook up other textures

    // finally, we need to add the layout for the geometry stream
    pipeline->beginGeometryStream();

    for (size_t i=0; i<vertexBuffer->vertexElementCount(); ++i) {
        pipeline->addVertexElement(*vertexBuffer->vertexElement(i));
    }

    pipeline->endGeometryStream();

    return pipeline;
}

} // namespace

namespace Teardrop {

TD_CLASS_IMPL(LandscapeScene);

LandscapeScene::LandscapeScene()
    : mSceneRenderer(nullptr)
    , mSceneRenderStep(nullptr)
    , mTerrainPipeline(nullptr)
{
}

LandscapeScene::~LandscapeScene()
{
    // the Renderables do not own their contents, so we need to
    // delete any meshes/materials we may have created
    delete mTerrainPipeline;
    delete mSceneRenderer;
}

bool LandscapeScene::initialize()
{
	return true;
}

bool LandscapeScene::destroy()
{
	return true;
}

bool LandscapeScene::onPreLoad()
{
    Logic* logic = getLogic();
    if (logic) {
        // invoke the logic first
        logic->onPreLoad(this);
    }

    return true;
}

bool LandscapeScene::onPostLoad()
{
    // do things to finalize scene loading
    generateTerrainTiles();

    // then invoke the logic, if set
    Logic* logic = getLogic();
    if (logic) {
        logic->onPostLoad(this);
    }

    mSceneRenderer = new SceneRenderer;
    mSceneRenderStep = new SceneRenderStep;
    mSceneRenderer->addStep(mSceneRenderStep);

    return true;
}

void LandscapeScene::tick()
{
    Logic* logic = getLogic();
    if (logic) {
        // tick the logic first
        logic->update(this);
    }

    // update anything else that needs updated
}

void LandscapeScene::renderFrame(Gfx::Renderer* renderer, Gfx::RenderTarget* rt)
{
    CameraController* camControl = getCameraController();
    if (!camControl) {
        return;
    }

    Gfx::Camera* camera = camControl->camera();
    if (!camera) {
        return;
    }

    if (!mRenderContext) {
        mRenderContext.reset(new RenderContext(renderer, rt));
    }

    camera->setAspect(rt->aspect());
    camera->update();

    mRenderContext->beginFrame(camera);

    ZoneObjects visibleObjects;
    getVisibleObjects(camera->getFrustumPlanes(), visibleObjects);
    mSceneRenderer->render(visibleObjects, mRenderContext.get(), this);

    mRenderContext->endFrame();
}

bool LandscapeScene::onPreUnload()
{
    delete mSceneRenderer;
    mSceneRenderer = nullptr;

    Logic* logic = getLogic();
    if (logic) {
        // invoke the logic first
        logic->onPreUnload(this);
    }

    // then start unloading ourselves
    return true;
}

bool LandscapeScene::onPostUnload()
{
    // do things to finalize scene unloading

    // then invoke the logic, if set
    Logic* logic = getLogic();
    if (logic) {
        logic->onPostUnload(this);
    }

    return true;
}

void LandscapeScene::injectMouseMove(int absX, int absY, int relX, int relY)
{
    CameraController* camControl = getCameraController();
    if (camControl) {
        camControl->injectMouseMove(absX, absY, relX, relY);
    }
}

void LandscapeScene::injectMouseWheel(int absZ, int relZ)
{
    CameraController* camControl = getCameraController();
    if (camControl) {
        camControl->injectMouseWheel(absZ, relZ);
    }
}

void LandscapeScene::generateTerrainTiles()
{
    Zone* zone = getCurrentZone();
    if (!zone) {
        return;
    }

    LandscapeAsset* asset = getLandscapeAsset();
    if (!asset) {
        return;
    }

    TextureAsset* hf = asset->getHeightField();
    if (!hf) {
        return;
    }

    int nTilesX = (int)getTiling().x;
    int nTilesY = (int)getTiling().y;

    for (int y = 0; y < nTilesY; ++y) {
        for (int x = 0; x < nTilesX; ++x) {
            TerrainTileComponent* tileComponent = TD_NEW TerrainTileComponent;
            tileComponent->initialize();
            tileComponent->initializeTile(x, y, this);

            if (!mTerrainPipeline) {
                // all of the tiles have the same vertex layout, so grab the tile and use it to
                // generate the terrain material
                Gfx::Renderable const& renderable = tileComponent->renderable();
                Gfx::Mesh const* mesh = renderable.mesh();
                Gfx::Submesh const* submesh = mesh->submesh(0);
                Gfx::VertexBuffer const* vb = submesh->vertexBuffer(0);
                mTerrainPipeline = createTerrainPipeline(asset, vb);
            }

            tileComponent->setPipeline(mTerrainPipeline);

            TerrainPatch* patch = TD_NEW TerrainPatch;
            patch->initialize();
            patch->addComponent(tileComponent);

            zone->addObject(patch);
        }
    }
}

} // namespace Teardrop
