/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "RenderWindow.h"
#include "Gfx/Renderer.h"
#include "Gfx/RenderTarget.h"
#include "Gfx/Camera.h"
#include "Gfx/Viewport.h"
#include <QTimer>
#include <QIcon>
#include <assert.h>

using namespace Teardrop;
using namespace Tools;

RenderWindow::RenderWindow(Gfx::Renderer* renderer, QWidget* parent/* =0 */)
	: QWidget(parent)
	, mTimer(0)
	, mRenderer(renderer)
	, mRT(0)
	, mCamera(0)
	, mViewport(0)
{
	mTimer = new QTimer(this);
	connect(mTimer, SIGNAL(timeout()), this, SLOT(onIdle()));
	mTimer->start(10);

	resize(1280, 800);
	setWindowTitle("Teardrop Editor - 3D View");
	show();

	assert(mRenderer);

	int flags = 
		Gfx::Renderer::INIT_FRAMEBUFFER_ALPHA |
		Gfx::Renderer::INIT_ENABLE_STENCIL_BUFFER |
		Gfx::Renderer::INIT_ENABLE_DEPTH_BUFFER;

	// initialize() will return a pointer to the first render target
	// created, which will be the "main" window
	mRT = mRenderer->initialize((uintptr_t)winId(), flags);
	assert(mRT);

	if (mRT) {
		mCamera = TD_NEW Gfx::Camera;
		mViewport = mRT->addViewport();
	}

	this->setWindowIcon(QIcon("icons/td-icon-32.png"));
}

RenderWindow::~RenderWindow()
{
	delete mCamera;

	if (mRT) {
		mRT->releaseViewport(mViewport);
	}
}

void RenderWindow::onIdle()
{
	assert(mRenderer);
	assert(mRT);

	// render a black clear frame for now
	if (mRenderer && mRT) {
		mRenderer->setRenderTarget(mRT);
		mRenderer->beginFrame();
		mRenderer->beginScene(mCamera, mViewport);
		mRenderer->endScene();
		mRenderer->endFrame();
	}
}
