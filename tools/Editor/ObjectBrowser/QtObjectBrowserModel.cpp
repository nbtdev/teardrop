/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "QtObjectBrowserModel.h"
#include "ProjectExplorer/QtProjectItem.h"
#include "PackageManager/PackageManager.h"
#include "PackageManager/PackageMetadata.h"
#include "PackageManager/Folder.h"
#include "PackageManager/Thumbnail.h"
#include "Asset/TextureAsset.h"
#include "QtUtils/TypeChooser.h"
#include <QImage>
#include <QPixmap>
#include <QMimeData>
#include <QUrl>
#include <QMessageBox>

using namespace Teardrop;
using namespace Tools;

static void populate(QList<QtProjectItem*>& list, QtProjectItem* parent)
{
	if (parent) {
		for (int i=0; i<parent->numChildren(); ++i) {
			populate(list, parent->child(i));
		}

		// then add all immediate object children of this folder item
		Folder* folder = parent->folder();
		if (folder) {
			PackageManager* pkgMgr = parent->packageManager();
			PackageMetadata* pkgMeta = pkgMgr->metadata();

			for (Objects::const_iterator it = folder->objects().begin(); it != folder->objects().end(); ++it) {
				Metadata* meta = pkgMeta->findObjectMetadata(*it);
				QtProjectItem* item = new QtProjectItem(pkgMgr, *it, meta, parent);
				list.append(item);
			}
		}
	}
}

QtObjectBrowserModel::QtObjectBrowserModel(QtProjectItem* topLevelItem, QObject* parent/* =0 */)
	: QAbstractListModel(parent)
	, mTopLevelItem(topLevelItem)
	, mRecursive(true)
{
	if (topLevelItem && topLevelItem->isFolder()) {
		Folder* folder = topLevelItem->folder();
		PackageManager* pkgMgr = topLevelItem->packageManager();
		PackageMetadata* pkgMeta = pkgMgr->metadata();

		if (folder) {
			// find all immediate object children of this folder
			for (Objects::const_iterator it = folder->objects().begin(); it != folder->objects().end(); ++it) {
				Metadata* meta = pkgMeta->findObjectMetadata(*it);
				QtProjectItem* item = new QtProjectItem(pkgMgr, *it, meta, topLevelItem);
				mImmediateChildren.append(item);
				mRecursiveChildren.append(item);
			}
		}

		// and then do a recursive find as well
		for (int i=0; i<topLevelItem->numChildren(); ++i) {
			QtProjectItem* child = topLevelItem->child(i);
			if (child->isFolder()) {
				populate(mRecursiveChildren, topLevelItem->child(i));
			}
		}
	}
}

QtObjectBrowserModel::~QtObjectBrowserModel()
{
	// we only need to delete the recursive list, as the 
	// immediate list is a proper subset of the recursive one
	qDeleteAll(mRecursiveChildren);
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

QStringList QtObjectBrowserModel::mimeTypes() const
{
	QStringList list = QAbstractItemModel::mimeTypes();
	list.append("text/plain");
	list.append("text/uri-list");
	return list;
}

Qt::DropActions QtObjectBrowserModel::supportedDropActions() const
{
	Qt::DropActions actions = QAbstractItemModel::supportedDropActions();
	return actions;
}

bool QtObjectBrowserModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
	if (data) {
		if (data->hasUrls()) {
			// dropping from filesystem onto "item"
			QList<QUrl> urls = data->urls();
			if (urls.length()) {
				// find out what type the user wants to import
				TypeChooser chooser(0, Asset::getClassDef());
				QDialog::DialogCode code = (QDialog::DialogCode)chooser.exec();

				if (code == QDialog::Accepted) {
					PackageManager* pkgMgr = mTopLevelItem->packageManager();
					//emit beginLongOperation();

					for (int i=0; i<urls.size(); ++i) {
						QString str = urls.at(i).toLocalFile();
						String pathName(str.toLatin1().constData());

						ImportedAsset importedAsset;
						pkgMgr->importAsset(importedAsset, mTopLevelItem->folder(), pathName, chooser.chosenClass());
						Asset* asset = importedAsset.asset();
						Metadata* assetMeta = importedAsset.metadata();

						if (asset) {
							emit layoutAboutToBeChanged();
							QtProjectItem* assetItem = new QtProjectItem(pkgMgr, asset, assetMeta, mTopLevelItem);
							mImmediateChildren.append(assetItem);
							if (mRecursive)
								mRecursiveChildren.append(assetItem);

							// there might be dependent assets imported as well...
							for (Dependencies::iterator it = importedAsset.dependencies().begin(); it != importedAsset.dependencies().end(); ++it) {
								Dependency& dep = *it;
								QtProjectItem* depItem = new QtProjectItem(pkgMgr, dep.mObject, dep.mMetadata, mTopLevelItem);
								mImmediateChildren.append(depItem);
								if (mRecursive)
									mRecursiveChildren.append(depItem);
							}

							emit layoutChanged();
						}
						else {
							QMessageBox mb;
							mb.setText(QString("Could not import ") + chooser.chosenClass()->getName() + QString(" from file ") + str);
							mb.exec();
						}
					}

					//emit endLongOperation();
				}
			}
		}
	}

	return true;
}

Qt::ItemFlags QtObjectBrowserModel::flags(const QModelIndex& index) const
{
	Qt::ItemFlags f = (Qt::ItemIsEnabled|Qt::ItemIsSelectable/*|Qt::ItemIsEditable*/);

	if (index.isValid()) {
		f |= Qt::ItemIsDragEnabled;
	}

	return f;
}

void QtObjectBrowserModel::remove(QtProjectItem* item)
{
	emit layoutAboutToBeChanged();
	mRecursiveChildren.removeOne(item);
	mImmediateChildren.removeOne(item);
	emit layoutChanged();
}