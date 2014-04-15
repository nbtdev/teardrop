/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "StaticMeshViewer.h"
#include "Asset/StaticMeshAsset.h"
#include "Gfx/Camera.h"
#include "Gfx/Mesh.h"
#include "Gfx/Renderer.h"
#include "Gfx/RenderTarget.h"
#include "Game/OrbitCamController.h"
#include "Math/Matrix44.h"
#include "Math/Transform.h"

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
	Gfx::Mesh* mesh = getStaticMeshAsset()->mesh();

	assert(mesh);

	if (!mesh)
		return;

	renderer->beginScene(cam, mVP);

	// apply the material
	renderer->apply(getStaticMeshAsset()->getMaterial());

	// begin the object
	renderer->beginObject(Matrix44::IDENTITY);

	// draw the submeshes
	int nSubmesh = mesh->submeshCount();
	for (int i=0; i<nSubmesh; ++i) {
		renderer->render(mesh->submesh(i));
	}

	renderer->endObject();
	renderer->endScene();
}
