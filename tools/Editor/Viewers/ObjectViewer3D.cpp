/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "ObjectViewer3D.h"
#include "PropertyGrid/PropertyGrid.h"
#include "Gfx/Renderer.h"
#include "Gfx/RenderTarget.h"
#include "Package/Executable.h"
#include "Package/Package.h"
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QSplitter>
#include <QtCore/QTimer>
#include <QtGui/QResizeEvent>
#include <assert.h>

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

	mRenderWindow = mRenderer->createRenderWindow((uintptr_t)m3DView->winId(), SURFACE_A8R8G8B8, INIT_ENABLE_DEPTH_BUFFER|INIT_ENABLE_STENCIL_BUFFER);
    if (!mRenderWindow) {
        QMessageBox mb;
        mb.setText("Could not create render window for static mesh asset viewer");
        mb.exec();
    }

	connect(mTimer, SIGNAL(timeout()), this, SLOT(onIdle()));
	mTimer->start(10);
}

ObjectViewer3D::~ObjectViewer3D()
{
	delete mTimer;
	delete mPropGrid;

	if (mRenderer) {
		mRenderer->setRenderTarget(0);
		mRenderer->releaseRenderTarget(mRenderWindow);
	}

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

void ObjectViewer3D::onIdle()
{
	if (mPackage) {
		Executable* exe = static_cast<Executable*>(mPackage->executable());

		if (exe && mRenderWindow && mRenderer) {
			exe->tick();

			mRenderer->setRenderTarget(mRenderWindow);
			mRenderer->beginFrame();
			exe->renderFrame(mRenderer, mRenderWindow);
			mRenderer->endFrame();
		}
	}
}

void ObjectViewer3D::mouseMoveEvent(QMouseEvent* event)
{

}
