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
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QTimer>
#include <assert.h>

using namespace Teardrop;
using namespace Tools;
using namespace Gfx;

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
	vl->addWidget(splitter);

	setAttribute(Qt::WA_DeleteOnClose);
	resize(800, 600);

	QString title("Object Viewer (3D)");
	//title.append(meta->getName());
	setWindowTitle(title);

	setWindowIcon(QIcon("icons/td-icon-32.png"));

	connect(mTimer, SIGNAL(timeout()), this, SLOT(onIdle()));
	mTimer->start(10);
}

ObjectViewer3D::~ObjectViewer3D()
{
	delete mTimer;
	delete mPropGrid;

	if (mRenderer) {
		mRenderer->releaseRenderTarget(mRenderWindow);
	}

	delete mPackage;
}

Package* ObjectViewer3D::package()
{
	if (!mPackage) {
		mPackage = TD_NEW Package;
	}

	return mPackage;
}

void ObjectViewer3D::onIdle()
{
	if (mPackage) {
		Executable* exe = static_cast<Executable*>(mPackage->executable());

		if (exe && mRenderWindow && mRenderer) {
			exe->tick();

			mRenderer->setRenderTarget(mRenderWindow);
			mRenderer->beginFrame();
			exe->renderFrame(mRenderer);
			mRenderer->endFrame();
		}
	}
}
