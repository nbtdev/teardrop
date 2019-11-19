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
#include "Core/Executable.h"
#include "Gfx/CommandBuffer.h"
#include "Gfx/CommandQueue.h"
#include "Gfx/Renderer.h"
#include "Gfx/RenderPass.h"
#include "Gfx/RenderTarget.h"
#include "Gfx/Camera.h"
#include "Gfx/Viewport.h"
#include <QtCore/QTimer>
#include <QtGui/QIcon>
#include <QMouseEvent>
#include <QWheelEvent>
#include <assert.h>
 
using namespace Teardrop;
using namespace Tools;

RenderWindow::RenderWindow(Gfx::Renderer* renderer, QWidget* parent/* =0 */)
	: QWidget(parent)
    , mTimer(nullptr)
	, mRenderer(renderer)
    , mRT(nullptr)
    , mCamera(nullptr)
    , mViewport(nullptr)
    , mExecutable(nullptr)
    , mLastMouseX(-1)
    , mLastMouseY(-1)
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

    // create "clear screen" render pass for use when no executable is present
    mClearPass = mRenderer->createRenderPass("EmptyBlackPass");
    mClearPass->attachOutput(mRT.get());
}

RenderWindow::~RenderWindow()
{
	delete mCamera;
}

void RenderWindow::setExecutable(Executable* executable)
{
    if (mExecutable) {
    }

    mExecutable = executable;
}

void RenderWindow::mouseMoveEvent(QMouseEvent* event)
{
    if (!mExecutable) {
        return;
    }

    // otherwise, inject the input
    int ax = event->pos().x();
    int ay = event->pos().y();

    if (mLastMouseX < 0) {
        mLastMouseX = ax;
    }

    if (mLastMouseY < 0) {
        mLastMouseY = ay;
    }

    int rx = mLastMouseX - ax;
    int ry = mLastMouseY - ay;

    mExecutable->injectMouseMove(ax, ay, rx, ry);
}

void RenderWindow::wheelEvent(QWheelEvent* event)
{
    if (!mExecutable) {
        return;
    }

    // otherwise, inject the input
    mExecutable->injectMouseWheel(event->delta(), event->delta());
}

void RenderWindow::onIdle()
{
    if (!mRenderer || !mRT) {
        return;
    }

    Gfx::CommandQueue* queue = mRenderer->getCommandQueue(0);
    if (!queue) {
        return;
    }

    mCamera->setAspect(mRT->aspect());
    mRT->setCurrent();
    mRT->clear(true, 0xFF000000);

    // if no executable, just leave it the black clear frame for now
    if (mExecutable) {
        mExecutable->tick();
        mExecutable->renderFrame(mRenderer, mRT.get());
    } else {
        std::unique_ptr<Gfx::CommandBuffer> cmdBuf = mRenderer->createCommandBuffer(false);
        cmdBuf->beginRecording();
        cmdBuf->beginRenderPass(mClearPass.get(), mRT.get(), nullptr);
        cmdBuf->endRecording();

        std::unique_ptr<Gfx::CommandQueue::Submission> submit = queue->createSubmission();
        mRenderer->getCommandQueue(0)->submit(submit.get(), 1, nullptr);
    }

    mRT->presentQueue(queue, nullptr, 0, nullptr);
}
