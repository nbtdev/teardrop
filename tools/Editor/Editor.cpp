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
{
	ui.setupUi(this);
	mPropGrid = new QtPropertyGrid(ui.dockWidgetContents);
	mPropGrid->setResizeMode(QtTreePropertyBrowser::Interactive);
	mPropGrid->setRootIsDecorated(true);
	mPropGrid->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	mPropGrid->clear();

	mPkgExp = new QtPackageExplorer(ui.dockWidgetContents);
	mPkgExp->setHeaderHidden(true);
	mPkgExp->setRootIsDecorated(true);

	ui.mPackageExplorerLayout->addWidget(mPkgExp);
	ui.mPackageExplorerLayout->addWidget(mPropGrid);

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