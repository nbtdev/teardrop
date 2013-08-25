/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "FolderItem.h"
#include "ObjectItem.h"
#include "PackageManager/Folder.h"
#include "PackageManager/PackageMetadata.h"
#include "PackageManager/PackageManager.h"
#include <assert.h>

using namespace Teardrop;
using namespace Tools;

FolderItem::FolderItem(FolderItem* parent, Folder* folder, PackageManager* pkgMgr)
	: mParent(parent)
	, mFolder(folder)
	, mPkgMgr(pkgMgr)
{
	const Objects& objs = folder->objects();
	for (Objects::const_iterator it = objs.begin(); it != objs.end(); ++it) {
		const String& id = pkgMgr->metadata()->findId(*it);
		assert(id.length());
		addObject(*it);
	}

	const Folders& folders = folder->folders();
	for (Folders::const_iterator it = folders.begin(); it != folders.end(); ++it) {
		addFolder(*it);
	}

	setText(0, (const char*)folder->name());
	setFlags(flags() | Qt::ItemIsEditable);

	folder->NameChanged.bind(this, &FolderItem::onNameChanged);
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

FolderItem* FolderItem::addFolder(Folder* folder)
{
	FolderItem* folderItem = new FolderItem(this, folder, mPkgMgr);
	mFolders.push_back(folderItem);
	addChild(folderItem);
	return folderItem;
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

void FolderItem::addObject(Reflection::Object* object)
{
	const String& objectId = mPkgMgr->metadata()->findId(object);
	ObjectItem* objItem = new ObjectItem(this, object, objectId);
	mObjects.push_back(objItem);
	addChild(objItem);
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

FolderItem* FolderItem::parent()
{
	return mParent;
}

PackageExplorerItem::Type FolderItem::itemType()
{
	return TYPE_FOLDER;
}

void FolderItem::onNameChanged(const char* newName)
{
	setText(0, newName);
}

PackageManager* FolderItem::packageManager()
{
	return mPkgMgr;
}
