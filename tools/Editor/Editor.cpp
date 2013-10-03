/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Editor.h"
#include "Project.h"
#include "ProgressDialog.h"
#include "RenderWindow.h"
#include "QtPropertyGrid/QtPropertyGrid.h"
#include "QtProjectExplorer/QtProjectExplorer.h"
#include "QtProjectExplorer/QtProjectItem.h"
#include "QtObjectBrowser/QtObjectBrowser.h"
#include "PackageManager/Metadata.h"
#include "Game/Scene.h"
#include "Game/TerrainZone.h"
#include "Package/Package.h"
#include "Util/FileSystem.h"
#include "FreeImage.h"
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
	, mProjectExp(0)
	, mObjBrowser(0)
	, m3DView(0)
	, mPropGridDesc(0)
	, mProject(0)
	, mRenderWindow(0)
{
	FreeImage_Initialise();

	ui.setupUi(this);
	mCursor = cursor();

	//m3DView = new QWidget(ui.centralWidget);
	//ui.horizontalLayout->addWidget(m3DView);
	this->setWindowIcon(QIcon("icons/td-icon-32.png"));

	mObjBrowser = new QtObjectBrowser(ui.centralWidget);
	ui.horizontalLayout->addWidget(mObjBrowser);

	QDockWidget* dock = new QDockWidget(this);
	QWidget* dockContents = new QWidget();
	QVBoxLayout* vertLayout = new QVBoxLayout(dockContents);

	mProjectExp = new QtProjectExplorer(dockContents);
	mProjectExp->setHeaderHidden(true);
	mProjectExp->setRootIsDecorated(true);
	vertLayout->addWidget(mProjectExp);
	dock->setWidget(dockContents);

	dock->setWindowTitle("Package Explorer");
	addDockWidget(Qt::RightDockWidgetArea, dock);

	dock = new QDockWidget(this);
	dockContents = new QWidget();
	vertLayout = new QVBoxLayout(dockContents);

	mPropGrid = new QtPropertyGrid(dockContents);
	//mPropGrid->setResizeMode(QtTreePropertyBrowser::Interactive);
	mPropGrid->setRootIsDecorated(true);
	mPropGrid->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	//mPropGrid->clear();

	vertLayout->addWidget(mPropGrid);
	dock->setWidget(dockContents);

	dock->setWindowTitle("Properties");
	addDockWidget(Qt::BottomDockWidgetArea, dock);

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

	connect(ui.mCmdNew, SIGNAL(triggered()), this, SLOT(onNew()));
	connect(ui.mCmdOpen, SIGNAL(triggered()), this, SLOT(onOpen()));
	connect(ui.mCmdSave, SIGNAL(triggered()), this, SLOT(onSave()));
	connect(ui.mCmdSaveAs, SIGNAL(triggered()), this, SLOT(onSaveAs()));
	connect(ui.mCmdPreferences, SIGNAL(triggered()), this, SLOT(onPreferences()));
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(onClose()));

	mProject = new Project;
	mProjectExp->setProject(mProject);

	mProjectExp->SelectionChanged.bind(this, &Editor::onProjectExplorerSelectionChanged);
	mObjBrowser->ItemClicked.bind(this, &Editor::onProjectExplorerSelectionChanged);
	mProjectExp->SelectionChanged.bind(mObjBrowser, &QtObjectBrowser::onItemSelected);

	setEditorTitle();
	mPreferences.load();

	// do we need to load the previous project?
	const Preferences::ProjectList& projs = mPreferences.projectList();
	if (mPreferences.general().mLoadLastProject && projs.size())
		openProject((const char*)projs.front());

	// set up render window
	mRenderWindow = new RenderWindow();

	// empty scene to start with
	mScene = TD_NEW Scene();
	Zone* zone = mScene->createZone(TerrainZone::getClassDef());
	mScene->setCurrentZone(*zone);
	mRenderWindow->setScene(mScene);
}

Editor::~Editor()
{
	mPreferences.save();
	delete mProject;
	FreeImage_DeInitialise();
}

void Editor::onClose()
{
	delete mRenderWindow;
	QMainWindow::close();
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
	delete mProject;
	mProject = new Project;
	setEditorTitle();
	mObjBrowser->clear();
	mProjectExp->setProject(mProject);
}

void Editor::openProject(const QString& file)
{
	QFile f(file);
	QFileInfo fi(f);
	QString path = fi.absolutePath();
	QString name = fi.baseName();
	QString ext = fi.suffix();

	// open the project
	Project* newProject = new Project;
	newProject->rename(name.toLatin1().data());
	newProject->setPath(path.toLatin1().data());

	ProgressDialog pd(this);
	pd.setMinimum(0);
	pd.setMaximum(100);
	pd.show();

	if (!newProject->read(&pd)) {
		// do something
		delete newProject;
	}
	else {
		mObjBrowser->clear();
		delete mProject;
		mProject = newProject;
		setEditorTitle();
		mProjectExp->setProject(mProject);
		mPreferences.addProject(file.toLatin1().data());
	}
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
		openProject(file);
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

void Editor::onPreferences()
{
	QDialog dlg(this);
	mDlgPreferences.setupUi(&dlg);
	mDlgPreferences.mChkOpenLastProject->setChecked(mPreferences.general().mLoadLastProject);

	QStringList renderers;
	renderers << "OpenGL" << "Direct3D 9" << "Direct3D 11";
	mDlgPreferences.mRenderEngine->insertItems(0, renderers);
	int selected = mPreferences.rendering().mEngine;
	mDlgPreferences.mRenderEngine->setCurrentIndex(selected);

	if (QDialog::Accepted == dlg.exec()) {
		mPreferences.general().mLoadLastProject = (mDlgPreferences.mChkOpenLastProject->checkState() != Qt::Unchecked);

		mPreferences.rendering().mEngine = (Preferences::Rendering::Engine)mDlgPreferences.mRenderEngine->currentIndex();

		mPreferences.save();
	}
}

void Editor::onProjectExplorerSelectionChanged(QtProjectItem* item)
{
	if (!item)
		mPropGrid->setObject(0);
	else
		mPropGrid->setObject(item->object(), item->metadata());
}