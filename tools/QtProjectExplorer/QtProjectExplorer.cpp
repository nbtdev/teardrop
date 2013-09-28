/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "QtProjectExplorer.h"
#include "QtProjectModel.h"
#include "QtProjectItem.h"
#include <QDragEnterEvent>
#include <QMenu>
#include "Asset/TextureAsset.h"

using namespace Teardrop;
using namespace Tools;

QtProjectExplorer::QtProjectExplorer(QWidget* parent)
	: QTreeView(parent)
	, mModel(0)
{
	setAcceptDrops(true);
	setDragEnabled(true);
	setDropIndicatorShown(true);
	setContextMenuPolicy(Qt::CustomContextMenu);

	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onContextMenu(const QPoint&)));
}

QtProjectExplorer::~QtProjectExplorer()
{
	delete mModel;
}

void QtProjectExplorer::setProject(Project* project)
{
	delete mModel;
	mModel = 0;

	if (project) {
		mModel = new QtProjectModel(project);
		setModel(mModel);
		expandAll();
	}
}

void QtProjectExplorer::dragMoveEvent(QDragMoveEvent* event)
{
	QModelIndex idx = indexAt(event->pos());
	if (idx.isValid()) {
		QtProjectItem* item = static_cast<QtProjectItem*>(idx.internalPointer());
		if (!item->isObject()) {
			event->acceptProposedAction();
			QTreeView::dragMoveEvent(event);
			return;
		}
	}

	event->ignore();
	QTreeView::dragMoveEvent(event);
}

void QtProjectExplorer::onContextMenu(const QPoint& pt)
{
	QtProjectModel* projModel = static_cast<QtProjectModel*>(model());

	// new context menu at location pt
	QPoint globalPt = mapToGlobal(pt);

	// what is underneath us?
	QModelIndex index = indexAt(pt);
	QtProjectItem* item = static_cast<QtProjectItem*>(index.internalPointer());
	if (item) {
		if (item->isFolder() || item->isPackage()) {
			QMenu menu;
			QAction* action = menu.addAction("Add Subfolder");
			action->setData(QVariant(1));
			action = menu.addAction("Delete");
			action->setData(QVariant(2));

			// add all available ("creatable") object types
			//QMenu* addObject = menu.addMenu("Add Object");
			//for (Reflection::ClassDef* classDef = Reflection::ClassDef::getClasses(); classDef; classDef = classDef->m_pNext) {
			//	if (classDef->isCreatable() && !classDef->isA(Asset::getClassDef())) {
			//		action = addObject->addAction(classDef->getName());
			//		action->setData(qVariantFromValue((void*)classDef));
			//	}
			//}

			action = menu.exec(globalPt);

			if (action) {
				switch(action->data().toInt()) {
				case 1:
					projModel->addFolder(index);
					break;
				case 2:
					{
						QModelIndex parentIndex = index.parent();
						QtProjectItem* parent = static_cast<QtProjectItem*>(parentIndex.internalPointer());

						projModel->deleteFolder(index);

						if (SelectionChanged)
							SelectionChanged(parent);
					}
					break;
				default: // it's a create-object-instance commmand, the menu item data is the ClassDef*
					projModel->addObject(index, (Reflection::ClassDef*)action->data().value<void*>());
					break;
				}
			}
		}
	}
	else {
		enum {
			ACTION_CREATE_PACKAGE,
		};

		QMenu menu;
		QMenu* createMenu = menu.addMenu("Create");
		QAction* action = createMenu->addAction("Package...");
		action->setData(QVariant(ACTION_CREATE_PACKAGE));

		action = menu.exec(globalPt);
		if (action) {
			switch(action->data().toInt()) {
			case ACTION_CREATE_PACKAGE:
				projModel->addPackage();
				break;
			}
		}
	}
}

void QtProjectExplorer::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
	if (SelectionChanged) {
		if (!current.isValid())
			SelectionChanged(0);
		else {
			QtProjectItem* item = static_cast<QtProjectItem*>(current.internalPointer());
			SelectionChanged(item);
		}
	}

	QTreeView::currentChanged(current, previous);
}
