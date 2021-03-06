/******************************************************************************
Copyright (c) 2015 Teardrop Games

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
******************************************************************************/

#include "ProjectModel.h"
#include "ProjectItem.h"
#include "Editor/Project.h"
#include "PackageManager/PackageManager.h"
#include "PackageManager/PackageMetadata.h"
#include "PackageManager/Folder.h"
#include "Asset/TextureAsset.h"
#include <QtCore/QMimeData>
#include <QtCore/QUrl>
#include <QtWidgets/QMessageBox>
#include "TypeChooser.h"
#include "Reflection/Reflection.h"
#include <stack>

using namespace Teardrop;
using namespace Tools;

static void populate(Folder* parentFolder, ProjectItem* parentItem, PackageManager* pkgMgr)
{
	if (parentFolder) {
		const Folders& folders = parentFolder->folders();
		for (Folders::const_iterator it = folders.begin(); it != folders.end(); ++it) {
			ProjectItem* folderItem = new ProjectItem(pkgMgr, *it, parentItem);
			populate(*it, folderItem, pkgMgr);
			parentItem->append(folderItem);
		}
	}

	//const Objects& objs = parentFolder->objects();
	//for (Objects::const_iterator it = objs.begin(); it != objs.end(); ++it) {
	//	parentItem->append(new ProjectItem(pkgMgr, *it, pkgMgr->metadata()->findObjectMetadata(*it), parentItem));
	//}
}

ProjectModel::ProjectModel(Project* project, QObject* parent /* = 0 */)
	: QAbstractItemModel(parent)
	, mProject(project)
	, mRoot(new ProjectItem)
{
	if (mProject) {
		const Project::PackageManagers& pkgMgrs = mProject->packages();
		for (Project::PackageManagers::const_iterator it = pkgMgrs.begin(); it != pkgMgrs.end(); ++it) {
			ProjectItem* rootItem = new ProjectItem(*it, mRoot);
			mRoot->append(rootItem);
			populate((*it)->metadata()->rootFolder(), rootItem, *it);
		}
	}	
}


ProjectModel::~ProjectModel()
{
	delete mRoot;
}

int ProjectModel::columnCount(const QModelIndex& parent) const
{
	return 1;
}

QVariant ProjectModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return QVariant();

	ProjectItem *item = static_cast<ProjectItem*>(index.internalPointer());

	if (!item)
		return QVariant();

	if (role == Qt::ToolTipRole) {
		return item->tooltip();
	}
	else if (role == Qt::FontRole) {
		if (item->active()) {
			QFont boldFont;
			boldFont.setBold(true);
			return boldFont;
		}
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

bool ProjectModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	ProjectItem* item = static_cast<ProjectItem*>(index.internalPointer());
	if (item) {
		if (role == Qt::EditRole) {
			item->rename(value.toString());
		}

		return true;
	}

	return false;
}

QModelIndex ProjectModel::index(int row, int column, const QModelIndex& parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	ProjectItem* parentItem;
	if (!parent.isValid())
		parentItem = mRoot;
	else parentItem = static_cast<ProjectItem*>(parent.internalPointer());

	ProjectItem* childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex ProjectModel::parent(const QModelIndex& index) const
{
	if (!index.isValid())
		return QModelIndex();

	ProjectItem* childItem = static_cast<ProjectItem*>(index.internalPointer());
	ProjectItem *parentItem = childItem->parent();

	if (parentItem == mRoot)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

int ProjectModel::rowCount(const QModelIndex& parent) const
{
	ProjectItem *parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = mRoot;
	else
		parentItem = static_cast<ProjectItem*>(parent.internalPointer());

	return parentItem->numChildren();
}

Qt::ItemFlags ProjectModel::flags(const QModelIndex& index) const
{
	Qt::ItemFlags f = (Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable);

	if (index.isValid()) {
		// valid index is an index over an item in the tree, set it draggable
		ProjectItem* item = static_cast<ProjectItem*>(index.internalPointer());
		if (!item->isPackage())
			f |= Qt::ItemIsDragEnabled;

		if (item->isPackage() || item->isFolder())
			f |= Qt::ItemIsDropEnabled;
	}
	else {

	}

	return f;
}

QMimeData* ProjectModel::mimeData(const QModelIndexList &indexes) const
{
	assert(indexes.size() <= 1);
	ProjectItem* item = static_cast<ProjectItem*>(indexes.at(0).internalPointer());
	if (item && !item->isPackage()) {
		QMimeData* mimeData = new QMimeData;
		mimeData->setData("application/x-qabstractitemmodeldatalist", "DRAG");
		mimeData->setUserData(0, new ProjectItemData(item));
		return mimeData;
	}

	return 0;
}

QStringList ProjectModel::mimeTypes() const
{
	QStringList list = QAbstractItemModel::mimeTypes();
	list.append("text/plain");
	list.append("text/uri-list");
	return list;
}

Qt::DropActions ProjectModel::supportedDropActions() const
{
	Qt::DropActions actions = QAbstractItemModel::supportedDropActions();
	return actions;
}

bool ProjectModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
	// this is the item being dropped upon
	ProjectItem* destItem = static_cast<ProjectItem*>(parent.internalPointer());

	if (data && destItem) {
		if (!data->hasUrls()) {
			// dragging/dropping from somewhere in the app; itemData is the item being dragged
			ProjectItemData* itemData = static_cast<ProjectItemData*>(data->userData(0));
			if (itemData) {
				ProjectItem* srcItem = itemData->item();

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
					ProjectItem* oldParent = srcItem->parent();
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

QVariant ProjectModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}

void ProjectModel::addFolder(const QModelIndex& parent)
{
	ProjectItem* parentItem = static_cast<ProjectItem*>(parent.internalPointer());

	if (parentItem) {
		emit layoutAboutToBeChanged();
		PackageManager* pkgMgr = parentItem->packageManager();
		PackageMetadata* pkgMeta = pkgMgr->metadata();
		Folder* parentFolder = parentItem->folder();

		Folder* newFolder = pkgMeta->newFolder("Untitled Folder", parentFolder);
		ProjectItem* newItem = new ProjectItem(pkgMgr, newFolder, parentItem);
		parentItem->append(newItem);
		emit layoutChanged();
	}
}

static void deleteItemRecursive(ProjectItem* item)
{
	// recursively deal with folder items
	for (int i=0; i<item->numChildren(); ++i) {
		ProjectItem* child = item->child(i);
		if (child->numChildren())
			deleteItemRecursive(child);

		// then delete the child
		delete child;
	}

	item->removeAllChildren();
}

void ProjectModel::deleteFolder(const QModelIndex& index, bool bRecursive)
{
	ProjectItem* item = static_cast<ProjectItem*>(index.internalPointer());

	if (item && !item->isRoot()) {
		emit layoutAboutToBeChanged();
		beginRemoveRows(index.parent(), index.row(), index.row());

		ProjectItem* parentItem = item->parent();
		PackageManager* pkgMgr = item->packageManager();
		Folder* folder = item->folder();

		if (bRecursive) {
			deleteItemRecursive(item);
		}
		else {
			// first adjust the model; all item's children go to the parent

			// TODO: keep the moved items with items of the same type in the parent (folders 
			// with folders, objects with objects)
			QList<ProjectItem*> children;
			for (int i=0; i<item->numChildren(); ++i) {
				children.append(item->child(i));
			}

			for (int i=0; i<children.size(); ++i) {
				ProjectItem* child = children.at(i);
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

void ProjectModel::addObject(const QModelIndex& parent, Reflection::ClassDef* classDef)
{
	ProjectItem* parentItem = static_cast<ProjectItem*>(parent.internalPointer());

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

		ProjectItem* newItem = new ProjectItem(pkgMgr, newObj, objMeta, parentItem);
		parentItem->append(newItem);
		emit layoutChanged();
	}
}

void ProjectModel::addPackage(const char* packagePath)
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
		ProjectItem* rootItem = new ProjectItem(pkgMgr, mRoot);
		mRoot->append(rootItem);
		populate(pkgMgr->metadata()->rootFolder(), rootItem, pkgMgr);
		emit layoutChanged();
	}
}

void ProjectModel::removePackage(const QModelIndex& package)
{
	ProjectItem* packageItem = static_cast<ProjectItem*>(package.internalPointer());
	beginRemoveRows(package.parent(), package.row(), package.row());
	emit layoutAboutToBeChanged();
	mRoot->remove(packageItem);
	mProject->removePackage(packageItem->packageManager());
	emit layoutChanged();
	endRemoveRows();
}

void ProjectModel::setActiveIndex(const QModelIndex& index)
{
	// only top-level indices can be set active, so go through each
	// and clear their active state, and set this one while we are at it
	ProjectItem* item = static_cast<ProjectItem*>(index.internalPointer());

	int nChildren = mRoot->numChildren();

	if (nChildren) {
		emit layoutAboutToBeChanged();
		for (int i=0; i<nChildren; ++i) {
			ProjectItem* child = mRoot->child(i);
			if (child == item)
				child->setActive(true);
			else
				child->setActive(false);
		}
		emit layoutChanged();
	}
}
