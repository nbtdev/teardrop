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

#include "RenderWindow.h"
#include "Gfx/Renderer.h"
#include "Gfx/RenderTarget.h"
#include "Gfx/Camera.h"
#include "Gfx/Viewport.h"
#include <QtCore/QTimer>
#include <QtGui/QIcon>
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
		Gfx::INIT_FRAMEBUFFER_ALPHA |
		Gfx::INIT_ENABLE_STENCIL_BUFFER |
		Gfx::INIT_ENABLE_DEPTH_BUFFER;

	// initialize() will return a pointer to the first render target
	// created, which will be the "main" window
	mRT = mRenderer->createRenderWindow((uintptr_t)winId(), Teardrop::Gfx::SURFACE_A8R8G8B8, flags);
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
}

void RenderWindow::onIdle()
{
	// render a black clear frame for now
	if (mRenderer && mRT) {
		mCamera->setAspect(mRT->aspect());
		mRT->setCurrent();
		mRT->clear(true, 0xFF000000);
		mRenderer->beginFrame();
		mRenderer->beginScene(mCamera, mViewport);
		mRenderer->endScene();
		mRenderer->endFrame();
		mRT->present();
	}
}
