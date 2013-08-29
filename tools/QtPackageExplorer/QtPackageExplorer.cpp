/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "QtPackageExplorer.h"
#include "FolderItem.h"
#include "QtUtils/TypeChooser.h"
#include "PackageManager/PackageManager.h"
#include "PackageManager/PackageMetadata.h"
#include "PackageManager/Folder.h"
#include "Package/Package.h"
#include "Asset/TextureAsset.h"
#include "Reflection/Reflection.h"
#include <QDragEnterEvent>
#include <QUrl>
#include <QMenu>

using namespace Teardrop;
using namespace Tools;

QtPackageExplorer::QtPackageExplorer(QWidget* parent)
	: QTreeWidget(parent)
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

void QtPackageExplorer::addPackage(PackageManager* packageMgr, bool setEditing)
{
	PackageMetadata* meta = packageMgr->metadata();
	FolderItem* folderItem = new FolderItem(0, meta->rootFolder(), packageMgr);
	addTopLevelItem(folderItem);
	setAutoExpandDelay(1);

	if (setEditing) {
		editItem(folderItem);
	}

	if (PackageAdded)
		PackageAdded(packageMgr);
}

void QtPackageExplorer::removePackage(PackageManager* packageMgr)
{
	if (PackageRemoved)
		PackageRemoved(packageMgr);
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

	if (mime->hasUrls()) {
		// dragging a file off of the filesystem
		QList<QUrl> urls = mime->urls();
		if (urls.length()) {
			QString str = urls.at(0).toLocalFile();

			QTreeWidgetItem* item = itemAt(event->pos());

			// if we are allowed to drop here, it's a folder
			FolderItem* folderItem = static_cast<FolderItem*>(item);

			// find out what type the user wants to import
			TypeChooser chooser(0, Asset::getClassDef());
			QDialog::DialogCode code = (QDialog::DialogCode)chooser.exec();

			if (code == QDialog::Accepted) {
				PackageManager* pkgMgr = folderItem->packageManager();
				Asset* asset = pkgMgr->importAsset(folderItem->folder(), str.toLatin1().constData(), chooser.chosenClass());
				if (asset) {
					folderItem->addObject(asset);
					event->accept();
					return;
				}
			}
		}
	}
	else {
		// moving a node to another parent in the tree
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

			action = menu.exec(globalPt);

			if (action) {
				switch(action->data().toInt()) {
			case 1:
				{
					PackageManager* pkgMgr = folderItem->packageManager();
					Folder* newFolder = pkgMgr->metadata()->newFolder("Untitled Folder", folderItem->folder());
					FolderItem* newItem = folderItem->addFolder(newFolder);
					expandItem(folderItem);
					editItem(newItem);
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