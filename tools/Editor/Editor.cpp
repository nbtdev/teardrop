/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Editor.h"
#include "Project.h"
#include "QtPropertyGrid/QtPropertyGrid.h"
#include "QtPackageExplorer/QtPackageExplorer.h"
#include "QtPackageExplorer/FolderItem.h"
#include "QtPackageExplorer/ObjectItem.h"
#include "PackageManager/Metadata.h"
#include "Package/Package.h"
#include "Util/FileSystem.h"
#include <QDockWidget>
#include <QVBoxLayout>
#include <QToolBox>
#include <QLabel>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>

using namespace Teardrop;
using namespace Tools;

Editor::Editor(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
	, mPropGrid(0)
	, mPkgExp(0)
	, m3DView(0)
	, mPropGridDesc(0)
	, mProject(0)
{
	ui.setupUi(this);
	mCursor = cursor();

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
	connect(ui.mCmdNew, SIGNAL(triggered()), this, SLOT(onNew()));
	connect(ui.mCmdOpen, SIGNAL(triggered()), this, SLOT(onOpen()));
	connect(ui.mCmdSave, SIGNAL(triggered()), this, SLOT(onSave()));
	connect(ui.mCmdSaveAs, SIGNAL(triggered()), this, SLOT(onSaveAs()));
	connect(mPkgExp, SIGNAL(beginLongOperation()), this, SLOT(onBeginLongOperation()));
	connect(mPkgExp, SIGNAL(endLongOperation()), this, SLOT(onEndLongOperation()));

	mProject = new Project;
	mPkgExp->PackageAdded.bind(mProject, &Project::onPackageAdded);
	mPkgExp->PackageRemoved.bind(mProject, &Project::onPackageRemoved);

	setEditorTitle();
}

Editor::~Editor()
{
	delete mProject;
}

void Editor::onPackageExplorerItemClicked(QTreeWidgetItem* item, int column)
{
	PackageExplorerItem* pkgItem = static_cast<PackageExplorerItem*>(item);
	if (pkgItem->itemType() == PackageExplorerItem::TYPE_OBJECT) {
		ObjectItem* objItem = static_cast<ObjectItem*>(item);
		mPropGrid->setObject(objItem->object(), objItem->metadata());
	}
	else {
		mPropGrid->setObject(0, 0);
	}
}

void Editor::onContextMenu(const QPoint& pt)
{

}

void Editor::setEditorTitle()
{
	String title("Teardrop Editor - ");
	title += mProject->name();
	setWindowTitle((const char*)title);
}

void Editor::onNew()
{
	// need to delete the current project and clear all packages from the pkg explorer widget
	delete mProject;
	mPkgExp->clearAllPackages();
	mProject = new Project;
	setEditorTitle();
}

void Editor::onOpen()
{
	// ask for filename and then do a normal save
	QFileDialog dlg;
	dlg.setFileMode(QFileDialog::AnyFile);
	dlg.setNameFilter(tr("Project Files (*.project)"));

	QStringList files;
	if (dlg.exec()) {
		files = dlg.selectedFiles();
		QString file = files.at(0);

		QFile f(files[0]);
		QFileInfo fi(f);
		QString path = fi.absolutePath();
		QString name = fi.baseName();
		QString ext = fi.suffix();

		// open the project
		Project* newProject = new Project;
		newProject->rename(name.toLatin1().data());
		newProject->setPath(path.toLatin1().data());

		if (!newProject->read()) {
			// do something
			delete newProject;
		}
		else {
			delete mProject;
			mProject = newProject;
			mPkgExp->PackageAdded.bind(mProject, &Project::onPackageAdded);
			mPkgExp->PackageRemoved.bind(mProject, &Project::onPackageRemoved);

			setEditorTitle();

			mPkgExp->clearAllPackages();
			const Project::PackageManagers& pkgMgrs = mProject->packages();
			for (Project::PackageManagers::const_iterator it = pkgMgrs.begin(); it != pkgMgrs.end(); ++it) {
				mPkgExp->_addPackage(*it);
			}
		}
	}
}

void Editor::onSave()
{
	// if project file exists, save it; if not, ask to save a new one
	if (!mProject)
		return;
	
	if (!mProject->name().length() || !mProject->path().length())
		onSaveAs();
	else {
		if (!mProject->write()) {
			// do something
		}
	}
}

void Editor::onSaveAs()
{
	// ask for filename and then do a normal save
	QFileDialog dlg;
	dlg.setFileMode(QFileDialog::AnyFile);
	dlg.setNameFilter(tr("Project Files (*.project)"));

	QStringList files;
	if (dlg.exec()) {
		files = dlg.selectedFiles();
		QString file = files.at(0);

		QFile f(files[0]);
		QFileInfo fi(f);
		QString path = fi.absolutePath();
		QString name = fi.baseName();
		QString ext = fi.suffix();

		// do the regular save
		mProject->rename(name.toLatin1().data());
		mProject->setPath(path.toLatin1().data());

		String pkgPath(mProject->path());
		pkgPath += "/packages";

		// make sure packages subdirectory is available
		if (FileSystem::createDirectory(pkgPath)) {
			if (!mProject->write()) {
				// do something
			}
		}

		setEditorTitle();
	}
}

void Editor::onBeginLongOperation()
{
	setCursor(Qt::WaitCursor);
	QApplication::processEvents();
}

void Editor::onEndLongOperation()
{
	setCursor(mCursor);
}