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

#include "Game/SceneRenderer.h"
#include "Game/SceneRenderStep.h"
#include "Game/ZoneObjects.h"
#include "Gfx/Camera.h"
#include "Gfx/Renderer.h"
#include "Gfx/RenderTarget.h"
#include "Gfx/Viewport.h"

#include <iostream>

using namespace Teardrop;

TD_CLASS_IMPL(LandscapeScene);

LandscapeScene::LandscapeScene()
    : mSceneRenderer(nullptr)
{
}

LandscapeScene::~LandscapeScene()
{
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
    mSceneRenderer->addStep(new SceneRenderStep);

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

    ZoneObjects visibleObjects;
    getVisibleObjects(camera->getFrustumPlanes(), visibleObjects);

    Gfx::Viewport* vp = rt->viewport();

    camera->setAspect(rt->aspect());
    rt->setCurrent();
    rt->clear(true, 0xFF000000);

    mSceneRenderer->render(visibleObjects, renderer, this, camera);

    rt->present();
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
    std::cerr << "Mouse: (" << absX << "," << absY << ")" << std::endl;

    CameraController* camControl = getCameraController();
    if (camControl) {
        camControl->injectMouseMove(absX, absY, relX, relY);
    }
}

void LandscapeScene::injectMouseWheel(int absZ, int relZ)
{
    std::cerr << "Wheel: (" << absZ << "," << relZ << ")" << std::endl;

    CameraController* camControl = getCameraController();
    if (camControl) {
        camControl->injectMouseWheel(absZ, relZ);
    }
}

void LandscapeScene::generateTerrainTiles()
{

}

