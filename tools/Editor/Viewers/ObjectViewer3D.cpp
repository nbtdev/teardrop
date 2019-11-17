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

#include "ObjectViewer3D.h"
#include "Core/Executable.h"
#include "Gfx/Renderer.h"
#include "Gfx/RenderTarget.h"
#include "Package/Package.h"
#include "PropertyGrid/PropertyGrid.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QSplitter>
#include <QtCore/QTimer>
#include <QtGui/QResizeEvent>

#include <cassert>

using namespace Teardrop;
using namespace Tools;
using namespace Gfx;

namespace {
    const char* sWindowName = "Object Viewer (3D)";
}

ObjectViewer3D::ObjectViewer3D(Gfx::Renderer* renderer, QWidget* parent/* =0 */)
	: QWidget(parent)
	, mRenderer(renderer)
	, mRenderWindow(0)
	, mPackage(0)
	, m3DView(0)
	, mPropGrid(0)
	, mTimer(new QTimer)
{
	assert(mRenderer);

	QVBoxLayout* vl = new QVBoxLayout(this);
	QSplitter* splitter = new QSplitter(this);
	m3DView = new QWidget(splitter);
	mPropGrid = new PropertyGrid(splitter);
	splitter->addWidget(m3DView);
	splitter->addWidget(mPropGrid);
	
	QList<int> sizes;
	int w = size().width();
	sizes << w * 4 / 5 << w / 5;
	
	splitter->setSizes(sizes);
	vl->addWidget(splitter);

	setAttribute(Qt::WA_DeleteOnClose);
	resize(800, 600);

    QString title(sWindowName);
	setWindowTitle(title);

	setWindowIcon(QIcon("icons/td-icon-32.png"));

	mRenderWindow = mRenderer->createRenderWindow((uintptr_t)m3DView->winId(), SURFACE_A8R8G8B8, INIT_FRAMEBUFFER_ALPHA|INIT_ENABLE_DEPTH_BUFFER|INIT_ENABLE_STENCIL_BUFFER);
    if (!mRenderWindow) {
        QMessageBox mb;
        mb.setText("Could not create render window for static mesh asset viewer");
        mb.exec();
    }

	connect(mTimer, SIGNAL(timeout()), this, SLOT(onIdle()));
	mTimer->start(33);
}

ObjectViewer3D::~ObjectViewer3D()
{
	delete mTimer;
	delete mPropGrid;

	if (mRenderWindow)
		mRenderWindow->unsetCurrent();

	delete mPackage;
}

void ObjectViewer3D::resizeEvent(QResizeEvent *event)
{
    if (mRenderWindow)
        mRenderWindow->resize(event->size().width(), event->size().height());
}

Package* ObjectViewer3D::package()
{
	if (!mPackage) {
		mPackage = TD_NEW Package;
	}

	return mPackage;
}

void ObjectViewer3D::setObjectName(const String& aName)
{
    QString title(sWindowName);
    title.append(" - ");
    title.append((const char*)aName);
    setWindowTitle(title);
}

void ObjectViewer3D::onClose()
{
    close();
}

void ObjectViewer3D::onIdle()
{
	if (mPackage) {
		Executable* exe = static_cast<Executable*>(mPackage->executable());

		if (exe && mRenderWindow && mRenderer) {
			exe->tick();

//			mRenderWindow->setCurrent();
//			mRenderWindow->clear();
//			mRenderer->beginFrame();
//			exe->renderFrame(mRenderer, mRenderWindow.get());
//			mRenderer->endFrame();
//			mRenderWindow->present();
		}
	}
}

void ObjectViewer3D::mouseMoveEvent(QMouseEvent* event)
{

}
