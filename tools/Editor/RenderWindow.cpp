/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "RenderWindow.h"
#include "Gfx/Renderer.h"
#include "Gfx/RenderTarget.h"
#include <QTimer>
#include <QIcon>
#include <assert.h>

using namespace Teardrop;
using namespace Tools;

RenderWindow::RenderWindow(QWidget* parent/* =0 */)
	: QWidget(parent)
	, mTimer(0)
	, mRenderer(0)
	, mRT(0)
{
	mTimer = new QTimer(this);
	connect(mTimer, SIGNAL(timeout()), this, SLOT(onIdle()));
	mTimer->start(10);

	resize(1280, 800);
	setWindowTitle("Teardrop Editor - 3D View");
	show();

	// obtain list of renderers, for now just pick first one if present
	const Gfx::RendererRegistration* regs = Gfx::rendererRegistrations();
	assert(regs);

	if (regs) {
		mRenderer = regs->create();
		assert(mRenderer);

		int flags = 
			Gfx::Renderer::INIT_FRAMEBUFFER_ALPHA |
			Gfx::Renderer::INIT_ENABLE_STENCIL_BUFFER |
			Gfx::Renderer::INIT_ENABLE_DEPTH_BUFFER;

		// initialize() will return a pointer to the first render target
		// created, which will be the "main" window
		mRT = mRenderer->initialize((uintptr_t)winId(), flags);
		assert(mRT);
	}

	this->setWindowIcon(QIcon("icons/td-icon-32.png"));
}

RenderWindow::~RenderWindow()
{
	if (mRenderer) {
		mRenderer->shutdown();
	}

	delete mRenderer;
}

void RenderWindow::onIdle()
{
	assert(mRenderer);
	assert(mRT);

	// render a black clear frame for now
	if (mRenderer && mRT) {
		mRenderer->setRenderTarget(mRT);
		mRenderer->beginFrame();
		mRenderer->beginScene();
		mRenderer->endScene();
		mRenderer->endFrame();
	}
}
