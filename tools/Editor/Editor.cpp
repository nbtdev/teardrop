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

	connect(mPkgExp, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(onPackageExplorerItemClicked(QTreeWidgetItem*,int)));
}

Editor::~Editor()
{

}

void Editor::onPackageExplorerItemClicked(QTreeWidgetItem* item, int column)
{
	PackageExplorerItem* pkgItem = static_cast<PackageExplorerItem*>(item);
	if (pkgItem->itemType() == PackageExplorerItem::TYPE_OBJECT) {
		ObjectItem* objItem = static_cast<ObjectItem*>(item);
		mPropGrid->setObject(objItem->object());
	}
	else {
		mPropGrid->setObject(0);
	}
}

void Editor::onContextMenu(const QPoint& pt)
{

}