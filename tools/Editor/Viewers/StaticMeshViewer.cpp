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

#include "StaticMeshViewer.h"
#include "Asset/MeshAsset.h"
#include "Gfx/Camera.h"
#include "Gfx/Exception.h"
#include "Gfx/Mesh.h"
#include "Gfx/PipelineStateManager.h"
#include "Gfx/Renderer.h"
#include "Gfx/RenderTarget.h"
#include "Game/OrbitCamController.h"
#include "Math/Matrix44.h"
#include "Math/Transform.h"
#include "Util/Environment.h"
#include "Util/Logger.h"

using namespace Teardrop;
using namespace Tools;

TD_CLASS_IMPL(StaticMeshViewer);

StaticMeshViewer::StaticMeshViewer()
	: mController(0)
	, mVP(0)
{
}

StaticMeshViewer::~StaticMeshViewer()
{
}

bool StaticMeshViewer::initialize()
{
	mTimer.reset();

	mController = TD_NEW OrbitCamController;
	mController->initialize();
	mController->setDistanceToTarget(30.f);

	return true;
}

void StaticMeshViewer::tick()
{
	if (mController)
		mController->update(mTimer.getElapsedTime());

	mTimer.reset();

	Executable::tick();
}

void StaticMeshViewer::renderFrame(Gfx::Renderer* renderer, Gfx::RenderTarget* rt)
{
	assert(mController);
	assert(getStaticMeshAsset());

	if (!mController)
		return;

	if (!getStaticMeshAsset())
		return;

	if (!mVP && rt) {
		mVP = rt->addViewport();
	}

	Gfx::Camera* cam = mController->camera();
    MeshAsset* meshAsset = getStaticMeshAsset();
    Gfx::Mesh* mesh = meshAsset->mesh();

	assert(mesh);

	if (!mesh)
		return;

	cam->setAspect(rt->aspect());

	try {
		renderer->beginScene(cam, mVP);

        size_t nSubmesh = mesh->submeshCount();

        if (nSubmesh > 0) {
            // set up the pipeline for this object
            renderer->apply(meshAsset->pipeline());

            // begin the object
            renderer->beginObject(Matrix44::IDENTITY);

            // draw the submeshes
            size_t nSubmesh = mesh->submeshCount();
            for (size_t i = 0; i < nSubmesh; ++i) {
                renderer->render(mesh->submesh(i));
            }

            renderer->endObject();
        }

		renderer->endScene();
	} catch (const Gfx::Exception& e) {
		Environment::get().pLogger->logMessage(e.what());
	}
}
