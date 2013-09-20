/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "QtObjectBrowserModel.h"
#include "QtProjectExplorer/QtProjectItem.h"
#include "PackageManager/Metadata.h"
#include "PackageManager/Thumbnail.h"
#include <QImage>
#include <QPixmap>
#include <QMimeData>

using namespace Teardrop;
using namespace Tools;

static void populate(QList<QtProjectItem*>& list, QtProjectItem* parent)
{
	if (parent) {
		for (int i=0; i<parent->numChildren(); ++i) {
			QtProjectItem* item = parent->child(i);
			if (item->isPackage() || item->isFolder())
				populate(list, item);
		}
	}

	// then add all immediate object children of this item
	for (size_t i=0; i<parent->numChildren(); ++i) {
		QtProjectItem* item = parent->child(i);
		if (item->isObject())
			list.append(item);
	}
}

QtObjectBrowserModel::QtObjectBrowserModel(QtProjectItem* topLevelItem, QObject* parent/* =0 */)
	: QAbstractListModel(parent)
	, mTopLevelItem(topLevelItem)
	, mRecursive(true)
{
	if (topLevelItem) {
		// find all immediate object children of this item
		for (size_t i=0; i<topLevelItem->numChildren(); ++i) {
			QtProjectItem* item = topLevelItem->child(i);
			if (item->isObject())
				mImmediateChildren.append(item);
		}

		// and then do a recursive find as well
		populate(mRecursiveChildren, topLevelItem);
	}
}

QtObjectBrowserModel::~QtObjectBrowserModel()
{

}

bool QtObjectBrowserModel::recursive() const
{
	return mRecursive;
}

void QtObjectBrowserModel::setRecursive(bool recursive)
{
	mRecursive = recursive;
}

QVariant QtObjectBrowserModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return QVariant();

	const QList<QtProjectItem*>* listp = mRecursive ? &mRecursiveChildren : &mImmediateChildren;
	const QList<QtProjectItem*>& list = *listp;

	if (index.row() >= list.size())
		return QVariant();

	QtProjectItem* item = list.at(index.row());
	if (role == Qt::DecorationRole) {
		Metadata* meta = item->metadata();
		const Thumbnail& thumb = meta->thumbnail();
		if (thumb.isValid()) {
			QPixmap pm;
			pm.loadFromData((const uchar*)thumb.data(), thumb.length());
			return pm;
		}
		else {
			// make a 128x128 icon from the default logo pixmap
			// todo: get this path from somewhere else?
			return QPixmap("icons/td-icon-128.png");
		}
	}

	if (role == Qt::DisplayRole || role == Qt::EditRole) {
		return item->toString();
	}

	return QVariant();
}

QModelIndex QtObjectBrowserModel::index(int row, int column, const QModelIndex& parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	if (column == 0) {
		if (row >= 0) {
			const QList<QtProjectItem*>* listp = mRecursive ? &mRecursiveChildren : &mImmediateChildren;
			const QList<QtProjectItem*>& list = *listp;

			if (row < list.size()) {
				return createIndex(row, column, list.at(row));
			}
		}
	}

	return QModelIndex();
}

int QtObjectBrowserModel::rowCount(const QModelIndex& parent) const
{
	return mRecursive ? mRecursiveChildren.size() : mImmediateChildren.size();
}

QMimeData* QtObjectBrowserModel::mimeData(const QModelIndexList &indexes) const
{
	assert(indexes.size() <= 1);
	QtProjectItem* item = static_cast<QtProjectItem*>(indexes.at(0).internalPointer());
	if (item) {
		QMimeData* mimeData = new QMimeData;
		mimeData->setData("application/x-qabstractitemmodeldatalist", "DRAG");
		mimeData->setUserData(0, new QtProjectItemData(item));
		return mimeData;
	}

	return 0;
}

bool QtObjectBrowserModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
	return false;
}

Qt::ItemFlags QtObjectBrowserModel::flags(const QModelIndex& index) const
{
	Qt::ItemFlags f = (Qt::ItemIsEnabled|Qt::ItemIsSelectable/*|Qt::ItemIsEditable*/);

	if (index.isValid()) {
		f |= Qt::ItemIsDragEnabled;
	}

	return f;
}