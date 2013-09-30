/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "QtProjectModel.h"
#include "QtProjectItem.h"
#include "Editor/Project.h"
#include "PackageManager/PackageManager.h"
#include "PackageManager/PackageMetadata.h"
#include "PackageManager/Folder.h"
#include "Asset/TextureAsset.h"
#include <QMimeData>
#include <QUrl>
#include <QMessageBox>
#include "QtUtils/TypeChooser.h"
#include "Reflection/Reflection.h"
#include <stack>

using namespace Teardrop;
using namespace Tools;

static void populate(Folder* parentFolder, QtProjectItem* parentItem, PackageManager* pkgMgr)
{
	if (parentFolder) {
		const Folders& folders = parentFolder->folders();
		for (Folders::const_iterator it = folders.begin(); it != folders.end(); ++it) {
			QtProjectItem* folderItem = new QtProjectItem(pkgMgr, *it, parentItem);
			populate(*it, folderItem, pkgMgr);
			parentItem->append(folderItem);
		}
	}

	//const Objects& objs = parentFolder->objects();
	//for (Objects::const_iterator it = objs.begin(); it != objs.end(); ++it) {
	//	parentItem->append(new QtProjectItem(pkgMgr, *it, pkgMgr->metadata()->findObjectMetadata(*it), parentItem));
	//}
}

QtProjectModel::QtProjectModel(Project* project, QObject* parent /* = 0 */)
	: QAbstractItemModel(parent)
	, mProject(project)
	, mRoot(new QtProjectItem)
{
	if (mProject) {
		const Project::PackageManagers& pkgMgrs = mProject->packages();
		for (Project::PackageManagers::const_iterator it = pkgMgrs.begin(); it != pkgMgrs.end(); ++it) {
			QtProjectItem* rootItem = new QtProjectItem(*it, mRoot);
			mRoot->append(rootItem);
			populate((*it)->metadata()->rootFolder(), rootItem, *it);
		}
	}	
}


QtProjectModel::~QtProjectModel()
{
	delete mRoot;
}

int QtProjectModel::columnCount(const QModelIndex& parent) const
{
	return 1;
}

QVariant QtProjectModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return QVariant();

	QtProjectItem *item = static_cast<QtProjectItem*>(index.internalPointer());

	if (!item)
		return QVariant();

	if (role == Qt::ToolTipRole) {
		return item->tooltip();
	}
	else if (role == Qt::DecorationRole) {
		QPixmap pm;
		if (item->isObject()) {
			pm.load("icons/td-icon-16.png");
		}
		else {
			pm.load("icons/td-folder-closed-16.png");
		}
		return pm;
	}
	else if (role == Qt::DisplayRole || role == Qt::EditRole) {
		return item->toString();
	}

	return QVariant();
}

bool QtProjectModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	QtProjectItem* item = static_cast<QtProjectItem*>(index.internalPointer());
	if (item) {
		if (role == Qt::EditRole) {
			item->rename(value.toString());
		}

		return true;
	}

	return false;
}

QModelIndex QtProjectModel::index(int row, int column, const QModelIndex& parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	QtProjectItem* parentItem;
	if (!parent.isValid())
		parentItem = mRoot;
	else parentItem = static_cast<QtProjectItem*>(parent.internalPointer());

	QtProjectItem* childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex QtProjectModel::parent(const QModelIndex& index) const
{
	if (!index.isValid())
		return QModelIndex();

	QtProjectItem* childItem = static_cast<QtProjectItem*>(index.internalPointer());
	QtProjectItem *parentItem = childItem->parent();

	if (parentItem == mRoot)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

int QtProjectModel::rowCount(const QModelIndex& parent) const
{
	QtProjectItem *parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = mRoot;
	else
		parentItem = static_cast<QtProjectItem*>(parent.internalPointer());

	return parentItem->numChildren();
}

Qt::ItemFlags QtProjectModel::flags(const QModelIndex& index) const
{
	Qt::ItemFlags f = (Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable);

	if (index.isValid()) {
		// valid index is an index over an item in the tree, set it draggable
		QtProjectItem* item = static_cast<QtProjectItem*>(index.internalPointer());
		if (!item->isPackage())
			f |= Qt::ItemIsDragEnabled;

		if (item->isPackage() || item->isFolder())
			f |= Qt::ItemIsDropEnabled;
	}
	else {

	}

	return f;
}

QMimeData* QtProjectModel::mimeData(const QModelIndexList &indexes) const
{
	assert(indexes.size() <= 1);
	QtProjectItem* item = static_cast<QtProjectItem*>(indexes.at(0).internalPointer());
	if (item && !item->isPackage()) {
		QMimeData* mimeData = new QMimeData;
		mimeData->setData("application/x-qabstractitemmodeldatalist", "DRAG");
		mimeData->setUserData(0, new QtProjectItemData(item));
		return mimeData;
	}

	return 0;
}

QStringList QtProjectModel::mimeTypes() const
{
	QStringList list = QAbstractItemModel::mimeTypes();
	list.append("text/plain");
	list.append("text/uri-list");
	return list;
}

Qt::DropActions QtProjectModel::supportedDropActions() const
{
	Qt::DropActions actions = QAbstractItemModel::supportedDropActions();
	return actions;
}

bool QtProjectModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
	// this is the item being dropped upon
	QtProjectItem* destItem = static_cast<QtProjectItem*>(parent.internalPointer());

	if (data && destItem) {
		if (data->hasUrls()) {
			// dropping from filesystem onto "item"
			QList<QUrl> urls = data->urls();
			if (urls.length()) {
				// find out what type the user wants to import
				TypeChooser chooser(0, Asset::getClassDef());
				QDialog::DialogCode code = (QDialog::DialogCode)chooser.exec();

				if (code == QDialog::Accepted) {
					PackageManager* pkgMgr = destItem->packageManager();
					//emit beginLongOperation();

					for (int i=0; i<urls.size(); ++i) {
						QString str = urls.at(i).toLocalFile();
						String pathName(str.toLatin1().constData());

						ImportedAsset importedAsset = pkgMgr->importAsset(destItem->folder(), pathName, chooser.chosenClass());
						Asset* asset = importedAsset.mAsset;
						Metadata* assetMeta = importedAsset.mMetadata;

						if (asset) {
							emit layoutAboutToBeChanged();
							QtProjectItem* assetItem = new QtProjectItem(pkgMgr, asset, assetMeta, destItem);
							destItem->append(assetItem);

							// there might be dependent assets imported as well...
							int nDeps = importedAsset.mNumDeps;
							for (int i=0; i<nDeps; ++i) {
								QtProjectItem* depItem = new QtProjectItem(pkgMgr, importedAsset.mDeps[i], importedAsset.mDepsMetadata[i], destItem);
								destItem->append(depItem);
							}

							emit layoutChanged();
						}
						else {
							QMessageBox mb;
							mb.setText(QString("Could not import texture from file ") + str);
							mb.exec();
						}
					}

					//emit endLongOperation();
				}
			}
		}
		else {
			// dragging/dropping from somewhere in the app; itemData is the item being dragged
			QtProjectItemData* itemData = static_cast<QtProjectItemData*>(data->userData(0));
			if (itemData) {
				QtProjectItem* srcItem = itemData->item();

				// check to see if the package managers are the same; if so, we only have to do an internal
				// move in the package
				PackageManager* srcPkgMgr = srcItem->packageManager();
				PackageManager* destPkgMgr = destItem->packageManager();

				if (srcPkgMgr == destPkgMgr && (destItem->isFolder() || destItem->isPackage())) {
					// just reparent internally
					Reflection::Object* obj = srcItem->object();
					Folder* folder = srcItem->folder();
					if (obj) {
						srcPkgMgr->metadata()->move(obj, srcItem->parent()->folder(), destItem->folder());
					}
					else if (folder) {
						srcPkgMgr->metadata()->move(folder, destItem->folder());
					}

					// and reparent the item itself
					emit layoutAboutToBeChanged();
					QtProjectItem* oldParent = srcItem->parent();
					oldParent->remove(srcItem);
					destItem->append(srcItem);
					emit layoutChanged();
				}
				else {
					// the hard way, have to remove from srcPkgMgr first
				}
			}
		}
	}

	return true;
}

QVariant QtProjectModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}

void QtProjectModel::addFolder(const QModelIndex& parent)
{
	QtProjectItem* parentItem = static_cast<QtProjectItem*>(parent.internalPointer());

	if (parentItem) {
		emit layoutAboutToBeChanged();
		PackageManager* pkgMgr = parentItem->packageManager();
		PackageMetadata* pkgMeta = pkgMgr->metadata();
		Folder* parentFolder = parentItem->folder();

		Folder* newFolder = pkgMeta->newFolder("Untitled Folder", parentFolder);
		QtProjectItem* newItem = new QtProjectItem(pkgMgr, newFolder, parentItem);
		parentItem->append(newItem);
		emit layoutChanged();
	}
}

static void deleteItemRecursive(QtProjectItem* item)
{
	// recursively deal with folder items
	for (int i=0; i<item->numChildren(); ++i) {
		QtProjectItem* child = item->child(i);
		if (child->numChildren())
			deleteItemRecursive(child);

		// then delete the child
		delete child;
	}

	item->removeAllChildren();
}

void QtProjectModel::deleteFolder(const QModelIndex& index, bool bRecursive)
{
	QtProjectItem* item = static_cast<QtProjectItem*>(index.internalPointer());

	if (item && !item->isRoot()) {
		emit layoutAboutToBeChanged();
		beginRemoveRows(index.parent(), index.row(), index.row());

		QtProjectItem* parentItem = item->parent();
		PackageManager* pkgMgr = item->packageManager();
		Folder* folder = item->folder();

		if (bRecursive) {
			deleteItemRecursive(item);
		}
		else {
			// first adjust the model; all item's children go to the parent

			// TODO: keep the moved items with items of the same type in the parent (folders 
			// with folders, objects with objects)
			QList<QtProjectItem*> children;
			for (int i=0; i<item->numChildren(); ++i) {
				children.append(item->child(i));
			}

			for (int i=0; i<children.size(); ++i) {
				QtProjectItem* child = children.at(i);
				item->remove(child);
				parentItem->append(child);
			}
		}

		parentItem->remove(item);
		delete item;

		// then do the package/metadata
		pkgMgr->remove(folder, bRecursive);

		endRemoveRows();
		emit layoutChanged();
	}
}

void QtProjectModel::addObject(const QModelIndex& parent, Reflection::ClassDef* classDef)
{
	QtProjectItem* parentItem = static_cast<QtProjectItem*>(parent.internalPointer());

	if (parentItem) {
		emit layoutAboutToBeChanged();
		PackageManager* pkgMgr = parentItem->packageManager();
		PackageMetadata* pkgMeta = pkgMgr->metadata();
		Folder* parentFolder = parentItem->folder();

		std::pair<Reflection::Object*, Metadata*> objPr = pkgMgr->createObject(parentFolder, classDef);
		Reflection::Object* newObj = objPr.first;
		Metadata* objMeta = objPr.second;

		String newName("Untitled ");
		newName.append(classDef->getName());
		objMeta->setName(newName);

		QtProjectItem* newItem = new QtProjectItem(pkgMgr, newObj, objMeta, parentItem);
		parentItem->append(newItem);
		emit layoutChanged();
	}
}

void QtProjectModel::addPackage(const char* packagePath)
{
	PackageManager* pkgMgr = 0;
	
	if (packagePath) {
		pkgMgr = mProject->addPackage(packagePath);
	}
	else {
		pkgMgr = mProject->createPackage();
	}

	if (pkgMgr) {
		emit layoutAboutToBeChanged();
		QtProjectItem* rootItem = new QtProjectItem(pkgMgr, mRoot);
		mRoot->append(rootItem);
		populate(pkgMgr->metadata()->rootFolder(), rootItem, pkgMgr);
		emit layoutChanged();
	}
}

void QtProjectModel::removePackage(const QModelIndex& package)
{
	QtProjectItem* packageItem = static_cast<QtProjectItem*>(package.internalPointer());
	beginRemoveRows(package.parent(), package.row(), package.row());
	emit layoutAboutToBeChanged();
	mRoot->remove(packageItem);
	mProject->removePackage(packageItem->packageManager());
	emit layoutChanged();
	endRemoveRows();
}