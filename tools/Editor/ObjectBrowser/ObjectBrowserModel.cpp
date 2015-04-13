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

#include "ObjectBrowserModel.h"
#include "ProjectExplorer/ProjectItem.h"
#include "PackageManager/AssetImportException.h"
#include "PackageManager/Folder.h"
#include "PackageManager/PackageManager.h"
#include "PackageManager/PackageMetadata.h"
#include "PackageManager/Thumbnail.h"
#include "Asset/TextureAsset.h"
#include "TypeChooser.h"
#include <QtGui/QImage>
#include <QtGui/QPixmap>
#include <QtCore/QMimeData>
#include <QtCore/QUrl>
#include <QtWidgets/QMessageBox>

using namespace Teardrop;
using namespace Tools;

static void populate(QList<ProjectItem*>& list, ProjectItem* parent)
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
				ProjectItem* item = new ProjectItem(pkgMgr, *it, meta, parent);
				list.append(item);
			}
		}
	}
}

ObjectBrowserModel::ObjectBrowserModel(ProjectItem* topLevelItem, QObject* parent/* =0 */)
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
				ProjectItem* item = new ProjectItem(pkgMgr, *it, meta, topLevelItem);
				mImmediateChildren.append(item);
				mRecursiveChildren.append(item);
			}
		}

		// and then do a recursive find as well
		for (int i=0; i<topLevelItem->numChildren(); ++i) {
			ProjectItem* child = topLevelItem->child(i);
			if (child->isFolder()) {
				populate(mRecursiveChildren, topLevelItem->child(i));
			}
		}
	}
}

ObjectBrowserModel::~ObjectBrowserModel()
{
	// we only need to delete the recursive list, as the 
	// immediate list is a proper subset of the recursive one
	qDeleteAll(mRecursiveChildren);
}

bool ObjectBrowserModel::recursive() const
{
	return mRecursive;
}

void ObjectBrowserModel::setRecursive(bool recursive)
{
	mRecursive = recursive;
}

QVariant ObjectBrowserModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return QVariant();

	const QList<ProjectItem*>* listp = mRecursive ? &mRecursiveChildren : &mImmediateChildren;
	const QList<ProjectItem*>& list = *listp;

	if (index.row() >= list.size())
		return QVariant();

	ProjectItem* item = list.at(index.row());
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

QModelIndex ObjectBrowserModel::index(int row, int column, const QModelIndex& parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	if (column == 0) {
		if (row >= 0) {
			const QList<ProjectItem*>* listp = mRecursive ? &mRecursiveChildren : &mImmediateChildren;
			const QList<ProjectItem*>& list = *listp;

			if (row < list.size()) {
				return createIndex(row, column, list.at(row));
			}
		}
	}

	return QModelIndex();
}

int ObjectBrowserModel::rowCount(const QModelIndex& parent) const
{
	return mRecursive ? mRecursiveChildren.size() : mImmediateChildren.size();
}

QMimeData* ObjectBrowserModel::mimeData(const QModelIndexList &indexes) const
{
	assert(indexes.size() <= 1);
	ProjectItem* item = static_cast<ProjectItem*>(indexes.at(0).internalPointer());
	if (item) {
		QMimeData* mimeData = new QMimeData;
		mimeData->setData("application/x-qabstractitemmodeldatalist", "DRAG");
		mimeData->setUserData(0, new ProjectItemData(item));
		return mimeData;
	}

	return 0;
}

QStringList ObjectBrowserModel::mimeTypes() const
{
	QStringList list = QAbstractItemModel::mimeTypes();
	list.append("text/plain");
	list.append("text/uri-list");
	return list;
}

Qt::DropActions ObjectBrowserModel::supportedDropActions() const
{
	Qt::DropActions actions = QAbstractItemModel::supportedDropActions();
	return actions;
}

bool ObjectBrowserModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
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

						try {
							ImportedAsset importedAsset;
							pkgMgr->importAsset(importedAsset, mTopLevelItem->folder(), pathName, chooser.chosenClass());
							Asset* asset = importedAsset.asset();
							Metadata* assetMeta = importedAsset.metadata();

							emit layoutAboutToBeChanged();

							ProjectItem* assetItem = new ProjectItem(pkgMgr, asset, assetMeta, mTopLevelItem);
							mImmediateChildren.append(assetItem);
							if (mRecursive)
								mRecursiveChildren.append(assetItem);

							// there might be dependent assets imported as well...
							for (Dependencies::iterator it = importedAsset.dependencies().begin(); it != importedAsset.dependencies().end(); ++it) {
								Dependency& dep = *it;
								ProjectItem* depItem = new ProjectItem(pkgMgr, dep.mObject, dep.mMetadata, mTopLevelItem);
								mImmediateChildren.append(depItem);
								if (mRecursive)
									mRecursiveChildren.append(depItem);
							}

							emit layoutChanged();
						}
						catch (const AssetImportException& e) {
							QString msg("Could not import ");
							msg += chooser.chosenClass()->getName();
							msg += "\nReason: ";
							msg += e.what();
							msg += "\nPath: ";
							msg += e.assetPath();

							QMessageBox mb;
							mb.setText(msg);
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

Qt::ItemFlags ObjectBrowserModel::flags(const QModelIndex& index) const
{
	Qt::ItemFlags f = (Qt::ItemIsEnabled|Qt::ItemIsSelectable/*|Qt::ItemIsEditable*/);

	if (index.isValid()) {
		f |= Qt::ItemIsDragEnabled;
	}

	return f;
}

ProjectItem* ObjectBrowserModel::addNew(Reflection::ClassDef* classDef)
{
	emit layoutAboutToBeChanged();

	// insert a new object into the package, at the top-level folder
	std::pair<Reflection::Object*, Metadata*> pr = mTopLevelItem->packageManager()->createObject(mTopLevelItem->folder(), classDef);

	// we want to make a new item and parent it to the top-level item from the project
	// explorer, but not append it to that list of children
	ProjectItem* item = new ProjectItem(mTopLevelItem->packageManager(), pr.first, pr.second, mTopLevelItem);

	// but we do want to append it to our child list(s)
	mRecursiveChildren.append(item);
	mImmediateChildren.append(item);
	emit layoutChanged();

	return item;
}

void ObjectBrowserModel::remove(ProjectItem* item)
{
	emit layoutAboutToBeChanged();
	mRecursiveChildren.removeOne(item);
	mImmediateChildren.removeOne(item);
	emit layoutChanged();
}
