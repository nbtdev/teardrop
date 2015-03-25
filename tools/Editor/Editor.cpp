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
#include "PropertyGrid/PropertyGrid.h"
#include "ProjectExplorer/ProjectExplorer.h"
#include "ProjectExplorer/ProjectItem.h"
#include "ObjectBrowser/ObjectBrowser.h"
#include "PackageManager/PackageMetadata.h"
#include "PackageManager/PackageManager.h"
#include "Package/Package.h"
#include "Game/Scene.h"
#include "Gfx/Renderer.h"
#include "Core/Input.h"
#include "Util/FileSystem.h"
#include "Util/Logger.h"
#include "FreeImage.h"
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QFileDialog>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtWidgets/QMessageBox>

#include "MaterialEditor/MaterialEditor.h"
#include "Viewers/ObjectViewer3D.h"
#include "Viewers/StaticMeshViewer.h"

#include "Gfx/Material.h"
#include "Asset/StaticMeshAsset.h"

using namespace Teardrop;
using namespace Tools;
using namespace std::placeholders;

Editor::Editor(QWidget *parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags)
	, mPropGrid(0)
	, mProjectExp(0)
	, mObjBrowser(0)
	, m3DView(0)
	, mPropGridDesc(0)
	, mProject(0)
	, mScene(0)
	, mRenderer(0)
	, mRenderWindow(0)
{
	FreeImage_Initialise();

	ui.setupUi(this);
	mCursor = cursor();

	this->setWindowIcon(QIcon("icons/td-icon-32.png"));

	mObjBrowser = new ObjectBrowser(ui.centralWidget);
	ui.horizontalLayout->addWidget(mObjBrowser);

	QDockWidget* dock = new QDockWidget(this);
	QWidget* dockContents = new QWidget();
	QVBoxLayout* vertLayout = new QVBoxLayout(dockContents);

	mProjectExp = new ProjectExplorer(dockContents);
	mProjectExp->setHeaderHidden(true);
	mProjectExp->setRootIsDecorated(true);
	vertLayout->addWidget(mProjectExp);
	dock->setWidget(dockContents);

	dock->setWindowTitle("Package Explorer");
	addDockWidget(Qt::RightDockWidgetArea, dock);

	dock = new QDockWidget(this);
	dockContents = new QWidget();
	vertLayout = new QVBoxLayout(dockContents);

	mPropGrid = new PropertyGrid(dockContents);
	mPropGrid->setRootIsDecorated(true);
	mPropGrid->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

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

    mProjectExp->SelectionChanged.bind(std::bind(&Editor::onProjectExplorerSelectionChanged, this, _1));
    mObjBrowser->ItemClicked.bind(std::bind(&Editor::onProjectExplorerSelectionChanged, this, _1));
    mObjBrowser->ItemDoubleClicked.bind(std::bind(&Editor::onObjectBrowserItemDoubleClicked, this, _1));
    mProjectExp->SelectionChanged.bind(std::bind(&ObjectBrowser::onItemSelected, mObjBrowser, _1));
	connect(mProjectExp, SIGNAL(activePackageChanged(PackageManager*)), this, SLOT(onActivePackageChanged(PackageManager*)));

	setEditorTitle();
	mPreferences.load();

	// create renderer instance
	// choose renderer from preferences...
	const Gfx::RendererRegistration* regs = Gfx::rendererRegistrations();
	assert(regs);

	while (regs) {
		if (regs->mUUID == mPreferences.rendering().mEngineId) {
			mRenderer = regs->create();
		}

		regs = regs->mNext;
	}

	// ...or first in list if no preference set (first time running, for example)
	if (!mRenderer) {
		if (regs) {
			mRenderer = regs[0].create();
		}
	}

	assert(mRenderer);

	if (mRenderer) {
		// set up render window
		mRenderWindow = new RenderWindow(mRenderer);

		// once we have created the initial render window, we can finish initializing the input system
		Input::instance().initialize((uintptr_t)mRenderWindow->winId());
	}

	// do we need to load the previous project?
	const Preferences::ProjectList& projs = mPreferences.projectList();
	if (mPreferences.general().mLoadLastProject && projs.size())
		openProject((const char*)projs.front());
}

Editor::~Editor()
{
	mPreferences.save();
	delete mProject;
	FreeImage_DeInitialise();

	if (mRenderer) {
		mRenderer->shutdown();
	}

	delete mRenderer;
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

	const Gfx::RendererRegistration* regs = Gfx::rendererRegistrations();
	assert(regs);

	int selected = -1, idx = 0;
	UUID selectedId = mPreferences.rendering().mEngineId;

	while (regs) {
		String uuid;
		regs->mUUID.toString(uuid);
		mDlgPreferences.mRenderEngine->insertItem(idx, regs->mDisplayName, QVariant((const char*)uuid));

		if (regs->mUUID == selectedId)
			selected = idx;

		regs = regs->mNext;
		idx++;
	}

	mDlgPreferences.mRenderEngine->setCurrentIndex(selected);

	if (QDialog::Accepted == dlg.exec()) {
		mPreferences.general().mLoadLastProject = (mDlgPreferences.mChkOpenLastProject->checkState() != Qt::Unchecked);

		UUID engineId;
		engineId.fromString(mDlgPreferences.mRenderEngine->currentData().toString().toLatin1().constData());
		mPreferences.rendering().mEngineId = engineId;

		mPreferences.save();
	}
}

void Editor::onProjectExplorerSelectionChanged(ProjectItem* item)
{
	if (!item)
		mPropGrid->setObject(0);
	else
		mPropGrid->setObject(item->object(), item->metadata());
}

void Editor::onObjectBrowserItemDoubleClicked(ProjectItem* item)
{
	// TODO: is there a way to do this more generically/elegantly?
	if (item) {
		Reflection::Object* obj = item->object();
		if (obj) {
			const Reflection::ClassDef* classDef = obj->getDerivedClassDef();

			// TODO: do this more automatically?
			if (classDef->isA(Gfx::Material::getClassDef())) {
				(new MaterialEditor(item))->show();
			}

			if (classDef->isA(StaticMeshAsset::getClassDef())) {
				StaticMeshAsset* sma = static_cast<StaticMeshAsset*>(obj);
				ObjectViewer3D* objViewer = TD_NEW ObjectViewer3D(mRenderer);

                // hook up signals for cleanup on exit
                connect(ui.actionExit, SIGNAL(triggered()), objViewer, SLOT(onClose()));

				Package* pkg = objViewer->package();
				Executable* exe = pkg->makeExecutable(StaticMeshViewer::getClassDef());

				StaticMeshViewer* smv = static_cast<StaticMeshViewer*>(exe);
				smv->initialize();
				smv->setStaticMeshAsset(sma);

                objViewer->setObjectName(item->metadata()->getName());

				objViewer->show();
			}
		}
	}
}

void Editor::onActivePackageChanged(PackageManager* pkgMgr)
{
	// check to see if the package has an Executable instance
	Executable* exe = pkgMgr->executable();

	if (!exe) {
		QMessageBox mb;
		mb.setText(QString("Package ") + pkgMgr->metadata()->getName() + QString(" must be set as 'executable'"));
		mb.exec();
	}
	else {
		//// for now, we know it's just Scene
		//Scene* scene = static_cast<Scene*>(list.front());
		//Scene* currentScene = mRenderWindow->scene();
		//if (currentScene)
		//	currentScene->destroy();

		//scene->initialize();
		//mRenderWindow->setScene(scene);
	}
}
