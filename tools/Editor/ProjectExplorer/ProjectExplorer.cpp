/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "ProjectExplorer.h"
#include "ProjectModel.h"
#include "ProjectItem.h"
#include "TypeChooser.h"
#include "PackageManager/PackageManager.h"
#include "PackageManager/PackageMetadata.h"
#include <QtGui/QDragEnterEvent>
#include <QtWidgets/QMenu>
#include <QtWidgets/QFileDialog>
#include "Asset/TextureAsset.h"
#include "Package/Executable.h"

using namespace Teardrop;
using namespace Tools;

ProjectExplorer::ProjectExplorer(QWidget* parent)
	: QTreeView(parent)
	, mModel(0)
{
	setAcceptDrops(true);
	setDragEnabled(true);
	setDropIndicatorShown(true);
	setContextMenuPolicy(Qt::CustomContextMenu);

	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onContextMenu(const QPoint&)));
}

ProjectExplorer::~ProjectExplorer()
{
	delete mModel;
}

void ProjectExplorer::setProject(Project* project)
{
	delete mModel;
	mModel = 0;

	if (project) {
		mModel = new ProjectModel(project);
		setModel(mModel);
		expandAll();
	}
}

void ProjectExplorer::dragMoveEvent(QDragMoveEvent* event)
{
	QModelIndex idx = indexAt(event->pos());
	if (idx.isValid()) {
		ProjectItem* item = static_cast<ProjectItem*>(idx.internalPointer());
		if (!item->isObject()) {
			event->acceptProposedAction();
			QTreeView::dragMoveEvent(event);
			return;
		}
	}

	event->ignore();
	QTreeView::dragMoveEvent(event);
}

void ProjectExplorer::onContextMenu(const QPoint& pt)
{
	ProjectModel* projModel = static_cast<ProjectModel*>(model());

	// new context menu at location pt
	QPoint globalPt = mapToGlobal(pt);

	// what is underneath us?
	QModelIndex index = indexAt(pt);
	ProjectItem* item = static_cast<ProjectItem*>(index.internalPointer());
	if (item) {
		if (item->isFolder() || item->isPackage()) {
			QMenu menu;
			QAction* action = menu.addAction("Add Subfolder");
			action->setData(QVariant(1));

			if (item->isPackage()) {
				action = menu.addAction("Remove From Project");
				action->setData(QVariant(3));
				action = menu.addAction("Set Active");
				action->setData(QVariant(5));
				action = menu.addAction("Make Executable");
				action->setData(QVariant(6));
			}
			else {
				action = menu.addAction("Delete");
				action->setData(QVariant(2));
				action = menu.addAction("Delete (Recursive)");
				action->setData(QVariant(4));
			}

			action = menu.exec(globalPt);

			bool bRecursive = true;
			if (action) {
				switch(action->data().toInt()) {
				case 1:
					projModel->addFolder(index);
					break;
				case 2:
					bRecursive = false;
				case 4:
					{
						QModelIndex parentIndex = index.parent();
						ProjectItem* parent = static_cast<ProjectItem*>(parentIndex.internalPointer());

						selectionModel()->clear();
						projModel->deleteFolder(index, bRecursive);
						setCurrentIndex(parentIndex);

                        SelectionChanged.raise(parent);
					}
					break;
				case 3: // remove package
					selectionModel()->clear();
					projModel->removePackage(index);
					setCurrentIndex(QModelIndex());

                    SelectionChanged.raise(0);
					break;
				case 5:
					projModel->setActiveIndex(index);
					emit activePackageChanged(item->packageManager());
					break;
				case 6:
					{
						TypeChooser chooser(0, Executable::getClassDef());
						QDialog::DialogCode code = (QDialog::DialogCode)chooser.exec();

						if (code == QDialog::Accepted) {
							// make a new Executable instance in this package
							PackageManager* pkgMgr = item->packageManager();
							pkgMgr->makeExecutable(chooser.chosenClass());
						}
					}
					break;
				}
			}
		}
	}
	else {
		enum {
			ACTION_CREATE_PACKAGE,
			ACTION_ADD_PACKAGE,
			ACTION_SET_ACTIVE,
		};

		QMenu menu;
		QAction* action = menu.addAction("New Package");
		action->setData(QVariant(ACTION_CREATE_PACKAGE));
		action = menu.addAction("Add Existing Package...");
		action->setData(QVariant(ACTION_ADD_PACKAGE));

		action = menu.exec(globalPt);
		if (action) {
			switch(action->data().toInt()) {
			case ACTION_CREATE_PACKAGE:
				projModel->addPackage();
				break;
			case ACTION_ADD_PACKAGE:
				{
					QFileDialog dlg;
					dlg.setFileMode(QFileDialog::ExistingFile);
					dlg.setNameFilter(tr("Package Files (*.package)"));
					QStringList files;
					if (dlg.exec()) {
						files = dlg.selectedFiles();
						QString file = files.at(0);
						String packagePath(file.toLatin1().data());
						projModel->addPackage(packagePath);
					}
				}
				break;
			}
		}
	}
}

void ProjectExplorer::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    if (!current.isValid())
        SelectionChanged.raise(0);
    else {
        ProjectItem* item = static_cast<ProjectItem*>(current.internalPointer());
        SelectionChanged.raise(item);
    }

	QTreeView::currentChanged(current, previous);
}
