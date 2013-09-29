/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "QtProjectItem.h"
#include "PackageManager/PackageManager.h"
#include "PackageManager/PackageMetadata.h"
#include "PackageManager/Folder.h"
#include "Reflection/Reflection.h"
#include "Package/Package.h"

using namespace Teardrop;
using namespace Tools;

QtProjectItem::QtProjectItem()
	: mPackageMgr(0)
	, mFolder(0)
	, mObject(0)
	, mMetadata(0)
	, mParent(0)
	, mIsPackage(0)
{
}

QtProjectItem::QtProjectItem(PackageManager* packageMgr, Reflection::Object* obj, Metadata* metadata, QtProjectItem* parent/* =0 */)
	: mPackageMgr(packageMgr)
	, mFolder(0)
	, mObject(obj)
	, mMetadata(metadata)
	, mParent(parent)
	, mIsPackage(0)
{
}

QtProjectItem::QtProjectItem(PackageManager* packageMgr, QtProjectItem* parent)
	: mPackageMgr(packageMgr)
	, mFolder(0)
	, mObject(0)
	, mMetadata(0)
	, mParent(parent)
	, mIsPackage(1)
{
	if (packageMgr) {
		mMetadata = packageMgr->metadata();
		mFolder = packageMgr->metadata()->rootFolder();
	}
}

QtProjectItem::QtProjectItem(PackageManager* packageMgr, Folder* folder, QtProjectItem* parent/* =0 */)
	: mPackageMgr(packageMgr)
	, mFolder(folder)
	, mObject(0)
	, mMetadata(0)
	, mParent(parent)
	, mIsPackage(0)
{
}

QtProjectItem::~QtProjectItem()
{
	qDeleteAll(mChildren);
}

bool QtProjectItem::isRoot() const
{
	// all other items must have mPackageMgr set
	return (mPackageMgr==0);
}

bool QtProjectItem::isPackage() const
{
	return mIsPackage != 0;
}

bool QtProjectItem::isFolder() const
{
	return (mFolder!=0);
}

bool QtProjectItem::isObject() const
{
	return (mObject!=0);
}

QtProjectItem* QtProjectItem::parent() const
{
	return mParent;
}

QtProjectItem* QtProjectItem::child(int row) const
{
	return mChildren.at(row);
}

int QtProjectItem::numChildren() const
{
	return mChildren.size();
}

int QtProjectItem::row() const
{
	if (mParent)
		return mParent->mChildren.indexOf(const_cast<QtProjectItem*>(this));

	return 0;
}

Reflection::Object* QtProjectItem::object() const
{
	return mObject;
}

Folder* QtProjectItem::folder() const
{
	return mFolder;
}

PackageManager* QtProjectItem::packageManager() const
{
	return mPackageMgr;
}

Metadata* QtProjectItem::metadata() const
{
	return mMetadata;
}

QString QtProjectItem::toString() const
{
	String sVal;
	if (mFolder) {
		sVal = mFolder->name();
	}
	else if (mMetadata) {
		sVal = mMetadata->getName();
	}

	return QString(sVal);
}

void QtProjectItem::append(QtProjectItem* child)
{
	mChildren.append(child);
	child->setParent(this);
}

void QtProjectItem::remove(QtProjectItem* child)
{
	mChildren.removeOne(child);
	child->setParent(0);
}

const String& QtProjectItem::path() const
{
	return mPath;
}

static void makePath(String& path, Folder* folder)
{
	if (folder) {
		makePath(path, folder->parent());

		path.append("/");
		path.append(folder->name());
	}
}

void QtProjectItem::updatePath()
{
	// only bother if it's an object; folder paths aren't displayed
	if (mObject) {
		mPath.clear();
		makePath(mPath, mParent->folder());
		mPath.append("/");
		mPath.append(mMetadata->getName());
	}

	// update the paths of all children (if any)
	for (int i=0; i<mChildren.size(); ++i) {
		mChildren[i]->updatePath();
	}
}

void QtProjectItem::setParent(QtProjectItem* newParent)
{
	mParent = newParent;

	if (mParent) {
		// update our path
		updatePath();
	}
}

QString QtProjectItem::tooltip() const
{
	return toString();
}

QtProjectItemData::QtProjectItemData(QtProjectItem* item)
	: mItem(item)
{

}

QtProjectItemData::~QtProjectItemData()
{

}

QtProjectItem* QtProjectItemData::item() const
{
	return mItem;
}

void QtProjectItem::rename(const QString& newName)
{
	String str(newName.toLatin1().data());
	if (isFolder())
		mPackageMgr->metadata()->renameFolder(mFolder, str);
	else
		mMetadata->setName(str);
}

void QtProjectItem::removeAllChildren()
{
	mChildren.clear();
}