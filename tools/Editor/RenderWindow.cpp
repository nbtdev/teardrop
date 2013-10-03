/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "RenderWindow.h"
#include "Gfx/GfxRenderer.h"
#include "Gfx/GfxCamera.h"
#include "Gfx/GfxViewport.h"
#include "Gfx/GfxRenderTarget.h"
#include "Game/SceneRenderer.h"
#include "Game/ShadowRenderStep.h"
#include "Game/SceneRenderStep.h"
#include "Game/Scene.h"
#include "Game/Zone.h"
#include "Math/MathUtil.h"
#include "Util/_String.h"
#include "Util/Logger.h"
#include "Util/Environment.h"
#include <QTimer>
#include <QIcon>

using namespace Teardrop;
using namespace Tools;

RenderWindow::RenderWindow(QWidget* parent/* =0 */)
	: QWidget(parent)
	, mTimer(0)
	, mRenderer(0)
	, mCamera(0)
	, mViewport(0)
	, mRT(0)
	, mScene(0)
	, mSceneRenderer(0)
{
	mTimer = new QTimer(this);
	connect(mTimer, SIGNAL(timeout()), this, SLOT(onIdle()));
	mTimer->start(10);

	resize(1280, 800);
	setWindowTitle("Teardrop Editor - 3D View");
	show();

	Environment& env = Environment::get();
	env.pRenderer = mRenderer = GfxRenderer::allocate(env, GetDEFAULTAllocator());

	GfxRenderConfig cfg;
	cfg.bNVPerfHUDAdapter = false;
	cfg.fullscreen = false;
	cfg.hWnd = winId();
	cfg.pLogger = env.pLogger;
	cfg.vsync = false;

	cfg.height = width();
	cfg.width = height();

	if (mRenderer->initialize(cfg)) {
		mRT = mRenderer->createRenderTarget(cfg, FMT_A8R8G8B8, GfxRenderer::RT_WINDOW);
		mViewport = mRT->addViewport(0, 0, 1, 1);

		mCamera = TD_NEW GfxCamera;
		mCamera->setAspect(float(width()) / float(height()));
		mCamera->setNearClip(1);
		mCamera->setFarClip(2500);
		mCamera->setFovY(MathUtil::HALF_PI/2);
		mCamera->setPosition(Vector4::UNIT_SCALE * 100);
		mCamera->setLookAt(Vector4::ZERO);

		mSceneRenderer = TD_NEW SceneRenderer;

		SceneRenderStep* step = TD_NEW SceneRenderStep;
		step->setCamera(mCamera);
		step->setViewport(mViewport);
		step->setRenderTarget(mRT);
		mSceneRenderer->addStep(step);
	}

	this->setWindowIcon(QIcon("icons/td-icon-32.png"));
}

RenderWindow::~RenderWindow()
{
	mRenderer->destroy();
	GfxRenderer::deallocate(mRenderer);
	Environment& env = Environment::get();
	env.pRenderer = 0;
}

void RenderWindow::setScene(Scene* scene)
{
	mScene = scene;
}

void RenderWindow::onIdle()
{
	if (mCamera)
		mCamera->update();

	if (mSceneRenderer && mScene) {
		ZoneObjects objects;
		mScene->getCurrentZone()->getVisibleObjects(
			mCamera->getFrustumPlanes(), objects);

		// render the scene
		mSceneRenderer->render(objects, mRenderer, mScene, mCamera);
	}
}

Scene* RenderWindow::scene()
{
	return mScene;
}