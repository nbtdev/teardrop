/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Editor.h"
#include "QtPropertyGrid/QtPropertyGrid.h"
#include "QtPackageExplorer/QtPackageExplorer.h"
#include "QtPackageExplorer/FolderItem.h"
#include "QtPackageExplorer/ObjectItem.h"
#include "Asset/Package.h"
#include <QDockWidget>
#include <QVBoxLayout>
#include <QToolBox>

// temp for test
#include "Game/MACRO.h"
#include "Game/Component_CameraInterface.h"
#include "Game/Component_Render.h"
#include "Game/Component_Audio.h"

using namespace Teardrop;
using namespace Tools;

Editor::Editor(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
	, mPropGrid(0)
	, mPkgExp(0)
	, m3DView(0)
{
	ui.setupUi(this);

	m3DView = new QWidget(ui.centralWidget);
	ui.horizontalLayout->addWidget(m3DView);

	QDockWidget* dock = new QDockWidget(this);
	QWidget* dockContents = new QWidget();
	QVBoxLayout* vertLayout = new QVBoxLayout(dockContents);

	mPkgExp = new QtPackageExplorer(dockContents);
	mPkgExp->setHeaderHidden(true);
	mPkgExp->setRootIsDecorated(true);
	vertLayout->addWidget(mPkgExp);
	dock->setWidget(dockContents);

	dock->setWindowTitle("Package Explorer");
	addDockWidget(Qt::RightDockWidgetArea, dock);

	dock = new QDockWidget(this);
	dockContents = new QWidget();
	vertLayout = new QVBoxLayout(dockContents);

	mPropGrid = new QtPropertyGrid(dockContents);
	mPropGrid->setResizeMode(QtTreePropertyBrowser::Interactive);
	mPropGrid->setRootIsDecorated(true);
	mPropGrid->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	mPropGrid->clear();

	vertLayout->addWidget(mPropGrid);
	dock->setWidget(dockContents);

	dock->setWindowTitle("Properties");
	addDockWidget(Qt::RightDockWidgetArea, dock);

	// object class list toolbox
	dock = new QDockWidget(this);
	dockContents = new QWidget();
	vertLayout = new QVBoxLayout(dockContents);
	mClasses = new QToolBox(dockContents);
	QWidget* page = new QWidget();
	mClasses->addItem(page, "Assets");
	page = new QWidget();
	mClasses->addItem(page, "Zone Objects");
	
	vertLayout->addWidget(mClasses);
	dock->setWidget(dockContents);
	dock->setWindowTitle("Classes");
	addDockWidget(Qt::RightDockWidgetArea, dock);

	// create some test data
	Package* pkg = new Package("Test");

	MACRO* macro = TD_NEW MACRO;
	macro->initialize();
	Folder* folder = pkg->root().newFolder("MACROs");
	folder->add(macro);

	Component* comp = TD_NEW RenderComponent;
	comp->initialize();
	macro->addComponent(comp);
	comp = TD_NEW AudioComponent;
	comp->initialize();
	macro->addComponent(comp);
	comp = TD_NEW CameraInterfaceComponent;
	comp->initialize();
	macro->addComponent(comp);

	mPropGrid->setObject(0);
	mPkgExp->addPackage(pkg);

	connect(mPkgExp, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(onPackageExplorerItemClicked(QTreeWidgetItem*,int)));
}

Editor::~Editor()
{

}

void Editor::onPackageExplorerItemClicked(QTreeWidgetItem* item, int column)
{
	// hack -- dyncast for now
	ObjectItem* oitem = dynamic_cast<ObjectItem*>(item);
	if (oitem) {
		mPropGrid->setObject(oitem->object());
	}
	else {
		mPropGrid->setObject(0);
	}
}