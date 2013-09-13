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
#include <QMimeData>
#include "QtUtils/ObjectDragDropData.h"
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

	const Objects& objs = parentFolder->objects();
	for (Objects::const_iterator it = objs.begin(); it != objs.end(); ++it) {
		parentItem->append(new QtProjectItem(pkgMgr, *it, pkgMgr->metadata()->findObjectMetadata(*it), parentItem));
	}
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

	if (role == Qt::ToolTipRole) {
		return item->tooltip();
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
	Qt::ItemFlags f = (Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEnabled);

	return f;
}

QStringList QtProjectModel::mimeTypes() const
{
	QStringList list = QAbstractItemModel::mimeTypes();
	list.append("text/plain");
	return list;
}

Qt::DropActions QtProjectModel::supportedDropActions() const
{
	Qt::DropActions actions = QAbstractItemModel::supportedDropActions();
	return actions;
}

bool QtProjectModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
	if (data) {
		QObjectUserData* od = data->userData(0);
		if (od) {
			QtProjectItem* item = static_cast<QtProjectItem*>(parent.internalPointer());
			if (item) {
				DragDropData* ddd = static_cast<DragDropData*>(od);
				if (ddd->type() == DragDropData::DDD_OBJECT) {
					ObjectDragDropData* oddd = static_cast<ObjectDragDropData*>(ddd);
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
