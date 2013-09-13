/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "QtPackageExplorer.h"
#include "FolderItem.h"
#include "ObjectItem.h"
#include "QtUtils/TypeChooser.h"
#include "QtUtils/ObjectDragDropData.h"
#include "QtUtils/FolderDragDropData.h"
#include "PackageManager/PackageManager.h"
#include "PackageManager/PackageMetadata.h"
#include "PackageManager/Folder.h"
#include "Package/Package.h"
#include "Asset/TextureAsset.h"
#include "Reflection/Reflection.h"
#include <QDragEnterEvent>
#include <QUrl>
#include <QMenu>
#include <QMessageBox>
#include <stack>

using namespace Teardrop;
using namespace Tools;

QtPackageExplorer::QtPackageExplorer(QWidget* parent)
	: QTreeWidget(parent)
	, mIsDragging(false)
{
	setAcceptDrops(true);
	setDragEnabled(true);
	setDropIndicatorShown(true);
	setContextMenuPolicy(Qt::CustomContextMenu);

	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onContextMenu(const QPoint&)));
	connect(this, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(onItemChanged(QTreeWidgetItem*, int)));
}

QtPackageExplorer::~QtPackageExplorer()
{
}

void QtPackageExplorer::_addPackage(PackageManager* packageMgr, bool setEditing)
{
	PackageMetadata* meta = packageMgr->metadata();
	FolderItem* folderItem = new FolderItem(0, meta->rootFolder(), packageMgr);
	addTopLevelItem(folderItem);
	setAutoExpandDelay(1);

	if (setEditing) {
		editItem(folderItem);
	}
}

void QtPackageExplorer::addPackage(PackageManager* packageMgr, bool setEditing)
{
	_addPackage(packageMgr, setEditing);

	if (PackageAdded)
		PackageAdded(packageMgr);
}

void QtPackageExplorer::removePackage(PackageManager* packageMgr)
{
	if (PackageRemoved)
		PackageRemoved(packageMgr);
}

void QtPackageExplorer::clearAllPackages()
{
	clear();
}

void QtPackageExplorer::dragEnterEvent(QDragEnterEvent* event)
{
	QTreeWidget::dragEnterEvent(event);
	event->acceptProposedAction();
}

void QtPackageExplorer::dragMoveEvent(QDragMoveEvent* event)
{
	QTreeWidgetItem* item = itemAt(event->pos());
	if (item) {
		setCurrentItem(item);

		PackageExplorerItem* pei = static_cast<PackageExplorerItem*>(item);
		if (pei->itemType() == PackageExplorerItem::TYPE_FOLDER) {
			event->acceptProposedAction();
			return;
		}
	}

	event->ignore();
}

void QtPackageExplorer::dragLeaveEvent(QDragLeaveEvent* event)
{
	event->accept();
}

bool QtPackageExplorer::dropMimeData(QTreeWidgetItem * newParentPtr, int index, const QMimeData * data, Qt::DropAction action)
{
	return QTreeWidget::dropMimeData(newParentPtr, index, data, action);
}

void QtPackageExplorer::dropEvent(QDropEvent* event)
{
	const QMimeData* mime = event->mimeData();
	QObjectUserData* od = 0;
	
	if (mime)
		od = mime->userData(0);

	if (mime->hasUrls()) {
		// dragging a file off of the filesystem
		QList<QUrl> urls = mime->urls();
		if (urls.length()) {
			QTreeWidgetItem* item = itemAt(event->pos());

			// if we are allowed to drop here, it's a folder
			FolderItem* folderItem = static_cast<FolderItem*>(item);

			// find out what type the user wants to import
			TypeChooser chooser(0, Asset::getClassDef());
			QDialog::DialogCode code = (QDialog::DialogCode)chooser.exec();

			if (code == QDialog::Accepted) {
				PackageManager* pkgMgr = folderItem->packageManager();
				emit beginLongOperation();

				for (int i=0; i<urls.size(); ++i) {
					QString str = urls.at(i).toLocalFile();

					Asset* asset = pkgMgr->importAsset(folderItem->folder(), str.toLatin1().constData(), chooser.chosenClass());
					if (asset) {
						folderItem->addObject(asset);
					}
					else {
						QMessageBox mb;
						mb.setText(QString("Could not import texture from file ") + str);
						mb.exec();
					}
				}

				emit endLongOperation();
				event->accept();
				return;
			}
		}
	}
	else if (od) {
		// moving a node to another parent in the tree
		DragDropData* ddd = static_cast<DragDropData*>(od);
		if (ddd->type() == DragDropData::DDD_OBJECT) {
			ObjectDragDropData* oddd = static_cast<ObjectDragDropData*>(ddd);
		}
		else if (ddd->type() == DragDropData::DDD_FOLDER) {
			FolderDragDropData* oddd = static_cast<FolderDragDropData*>(ddd);
		}
	}

	event->ignore();
}

void QtPackageExplorer::onContextMenu(const QPoint& pt)
{
	// new context menu at location pt
	QPoint globalPt = mapToGlobal(pt);

	// what is underneath us?
	QTreeWidgetItem* item = itemAt(pt);
	if (item) {
		PackageExplorerItem* pkgItem = static_cast<PackageExplorerItem*>(item);
		if (pkgItem->itemType() == PackageExplorerItem::TYPE_FOLDER) {
			FolderItem* folderItem = static_cast<FolderItem*>(item);

			QMenu menu;
			QAction* action = menu.addAction("Add Subfolder");
			action->setData(QVariant(1));

			// add all available ("creatable") object types
			QMenu* addObject = menu.addMenu("Add Object");
			for (Reflection::ClassDef* classDef = Reflection::ClassDef::getClasses(); classDef; classDef = classDef->m_pNext) {
				if (classDef->isCreatable() && !classDef->isA(Asset::getClassDef())) {
					action = addObject->addAction(classDef->getName());
					action->setData(qVariantFromValue((void*)classDef));
				}
			}

			action = menu.exec(globalPt);

			if (action) {
				PackageManager* pkgMgr = folderItem->packageManager();
				switch(action->data().toInt()) {
				case 1:
				{
					Folder* newFolder = pkgMgr->metadata()->newFolder("Untitled Folder", folderItem->folder());
					FolderItem* newItem = folderItem->addFolder(newFolder);
					expandItem(folderItem);
					editItem(newItem);
				}
					break;
				default: // it's a create-object-instance commmand, the menu item data is the ClassDef*
				{
					Reflection::ClassDef* classDef = (Reflection::ClassDef*)action->data().value<void*>();
					Reflection::Object* obj = pkgMgr->createObject(folderItem->folder(), classDef);
					ObjectItem* objItem = folderItem->addObject(obj);
					expandItem(folderItem);
					editItem(objItem);
				}
					break;
				}
			}
		}
		else if (pkgItem->itemType() == PackageExplorerItem::TYPE_OBJECT) {
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
			{
				// TODO: this can't happen here, packages belong to the Project class, we need to
				// emit a signal to have the project create the class and add it to the package explorer
				PackageManager* pkgMgr = new PackageManager;
				pkgMgr->metadata()->setName("Untitled Package");
				mPackages.push_back(pkgMgr);
				addPackage(pkgMgr, true);
			}
			break;
			}
		}
	}
}

void QtPackageExplorer::onItemChanged(QTreeWidgetItem *item, int column) {
	PackageExplorerItem* pei = static_cast<PackageExplorerItem*>(item);
	pei->labelChanged(String(item->text(0).toLatin1().data()));
}

static void constructPath(FolderItem* fi, String& path)
{
	if (fi) {
		constructPath(fi->parent(), path);
	
		if (path.length())
			path.append("/");

		path.append(fi->folder()->name());
	}
}

void QtPackageExplorer::mouseMoveEvent(QMouseEvent* event)
{
	if (!(event->buttons() & Qt::LeftButton)) {
		if (mIsDragging) {
			mIsDragging = false;
			return;
		}
	}
	else {
		if (mIsDragging) {
			int dist = (event->pos() - mMouseDown).manhattanLength();
			if (dist < 4/*QApplication::startDragDistance()*/) {
				return;
			}
		}
		else {
			mMouseDown = event->pos();
			mIsDragging = true;
		}
	}

	// get thing that we are dragging (it will be where the mouse drag started)
	PackageExplorerItem* pei = static_cast<PackageExplorerItem*>(itemAt(mMouseDown.x(), mMouseDown.y()));

	if (pei) {
		DragDropData* ddd = 0;

		if (pei->itemType() == PackageExplorerItem::TYPE_OBJECT) {
			ObjectItem* oi = static_cast<ObjectItem*>(pei);

			// construct text "path" to object in folder hierarchy
			String path;
			FolderItem* fi = oi->parent();
			constructPath(fi, path);

			path.append("/");
			path.append(oi->metadata()->getName());

			ddd = new ObjectDragDropData(oi->object(), path, 0);
		}
		else if (pei->itemType() == PackageExplorerItem::TYPE_FOLDER) {
			FolderItem* fi = static_cast<FolderItem*>(pei);

			// construct text "path" to folder in hierarchy
			String path;
			FolderItem* fip = fi->parent();
			constructPath(fip, path);

			path.append("/");
			path.append(fi->folder()->name());

			ddd = new FolderDragDropData(fi->folder(), path, 0);
		}

		if (ddd) {
			onDragStart(ddd);
			mIsDragging = false;
		}
	}

	QTreeWidget::mouseMoveEvent(event);
}

void QtPackageExplorer::onDragStart(DragDropData* ddd)
{
	QDrag* drag = new QDrag(this);

	// because Qt insists that everything has to have a MIME type...
	QMimeData* mimeData = new QMimeData;
	mimeData->setData("application/x-qabstractitemmodeldatalist", "DRAG");
	mimeData->setUserData(0, ddd);
	drag->setMimeData(mimeData);

	drag->exec(Qt::CopyAction);
}