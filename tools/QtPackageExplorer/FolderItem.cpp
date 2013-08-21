/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "FolderItem.h"
#include "ObjectItem.h"
#include "Asset/Package.h"

using namespace Teardrop;
using namespace Tools;

FolderItem::FolderItem(PackageItem* package, Folder* folder)
	: mParent(0)
	, mOwner(package)
	, mFolder(folder)
{
	const Objects& objs = folder->objects();
	for (Objects::const_iterator it = objs.begin(); it != objs.end(); ++it) {
		addObject(new ObjectItem(this, (*it)));
	}

	const Folders& folders = folder->folders();
	for (Folders::const_iterator it = folders.begin(); it != folders.end(); ++it) {
		addFolder(new FolderItem(this, (*it)));
	}

	setText(0, (const char*)folder->name());
}

FolderItem::FolderItem(FolderItem* parent, Folder* folder)
	: mParent(parent)
	, mOwner(0)
	, mFolder(folder)
{
	const Objects& objs = folder->objects();
	for (Objects::const_iterator it = objs.begin(); it != objs.end(); ++it) {
		addObject(new ObjectItem(this, (*it)));
	}

	const Folders& folders = folder->folders();
	for (Folders::const_iterator it = folders.begin(); it != folders.end(); ++it) {
		addFolder(new FolderItem(this, (*it)));
	}

	setText(0, (const char*)folder->name());
}

FolderItem::~FolderItem()
{
	for (ObjectItems::iterator it = mObjects.begin(); it != mObjects.end(); ++it)
		delete *it;

	for (FolderItems::iterator it = mFolders.begin(); it != mFolders.end(); ++it)
		delete *it;
}

int FolderItem::numFolders() const
{
	return int(mFolders.size());
}

FolderItem* FolderItem::folder(int idx)
{
	if (idx >= 0 & idx < mFolders.size()) {
		for (FolderItems::iterator it = mFolders.begin(); it != mFolders.end() && idx >= 0; ++it) {
			if (--idx == 0)
				return *it;
		}
	}

	return 0;
}

void FolderItem::addFolder(FolderItem* folder)
{
	mFolders.push_back(folder);
	addChild(folder);
}

void FolderItem::removeFolder(FolderItem* folder)
{
	for (FolderItems::iterator it = mFolders.begin(); it != mFolders.end(); ++it) {
		if (*it == folder) {
			mFolders.erase(it);
			break;
		}
	}
}

int FolderItem::numObjects() const
{
	return int(mObjects.size());
}

ObjectItem* FolderItem::object(int idx)
{
	if (idx >= 0 & idx < mObjects.size()) {
		for (ObjectItems::iterator it = mObjects.begin(); it != mObjects.end() && idx >= 0; ++it) {
			if (--idx == 0)
				return *it;
		}
	}

	return 0;
}

void FolderItem::addObject(ObjectItem* object)
{
	mObjects.push_back(object);
	addChild(object);
}

void FolderItem::removeObject(ObjectItem* object)
{
	for (ObjectItems::iterator it = mObjects.begin(); it != mObjects.end(); ++it) {
		if (*it == object) {
			mObjects.erase(it);
			break;
		}
	}
}

Folder* FolderItem::folder()
{
	return mFolder;
}

PackageItem* FolderItem::owner()
{
	return mOwner;
}

FolderItem* FolderItem::parent()
{
	return mParent;
}
