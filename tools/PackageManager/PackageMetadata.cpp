/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "PackageMetadata.h"
#include "Folder.h"
#include "Uuid.h"
#include "Asset/Package.h"
#include "Asset/Asset.h"

using namespace Teardrop;
using namespace Tools;

PackageMetadata::PackageMetadata(Package* package)
	: mPackage(package)
{
	mRoot = new Folder(mPackageName, 0);
}

PackageMetadata::~PackageMetadata()
{

}

Folder* PackageMetadata::rootFolder()
{
	return mRoot;
}

void PackageMetadata::add(String& uuid, Folder* parent, Asset* asset, const char* assetSourcePath)
{
	// call the general object insert handler first...
	add(uuid, parent, asset);

	// this is the only special treatment that assets get...
	if (assetSourcePath) {
		mAssetIdToPathMap[uuid] = String(assetSourcePath);
		mAssetToPathMap[asset] = String(assetSourcePath);
	}

	// and actually add it to the folder
	parent->add(asset);
}

void PackageMetadata::add(String& uuid, Folder* parent, Reflection::Object* object)
{
	UuidGen(uuid);

	// map "object <--> ID"
	mObjectIdToObjectMap[uuid] = object;
	mObjectToObjectIdMap[object] = uuid;

	const_cast<Folder*>(parent)->add(object);
	mObjectToFolderMap[object] = parent;
}

void PackageMetadata::remove(const String& objectId)
{
	// remove from "asset ID --> asset path" map
	mAssetIdToPathMap.erase(objectId);

	// and then the others
	ObjectIdToObjectMap::iterator it = mObjectIdToObjectMap.find(objectId);
	if (it != mObjectIdToObjectMap.end()) {
		Reflection::Object* obj = it->second;

		// remove from this map 
		mObjectIdToObjectMap.erase(it);

		// and the "object --> object ID" map
		mObjectToObjectIdMap.erase(obj);

		// and, if exists, the "asset --> path" map
		if (obj->getDerivedClassDef()->isA(Asset::getClassDef())) {
			mAssetToPathMap.erase(static_cast<Asset*>(obj));
		}

		// "object --> folder" map...
		mObjectToFolderMap.erase(obj);
	}
}

void PackageMetadata::remove(Reflection::Object* object)
{
	// and "asset --> path" map
	if (object->getDerivedClassDef()->isA(Asset::getClassDef())) {
		mAssetToPathMap.erase(static_cast<Asset*>(object));
	}

	// "object --> folder" map...
	mObjectToFolderMap.erase(object);

	ObjectToObjectIdMap::iterator it = mObjectToObjectIdMap.find(object);
	if (it != mObjectToObjectIdMap.end()) {
		String id(it->second);

		// remove me from this map
		mObjectToObjectIdMap.erase(it);

		// and the "asset ID --> path" map
		mAssetIdToPathMap.erase(id);

		// and the "object ID --> object" map
		mObjectIdToObjectMap.erase(id);
	}
}

void PackageMetadata::move(Reflection::Object* object, Folder* oldParent, Folder* newParent)
{
	if (!object || !oldParent || !newParent)
		return;

	oldParent->remove(object);
	newParent->add(object);

	// then update the object-->folder table
	mObjectToFolderMap[object] = newParent;
}

const String& PackageMetadata::findAssetSourcePath(const String& assetId)
{
	AssetIdToPathMap::iterator it = mAssetIdToPathMap.find(assetId);
	if (it == mAssetIdToPathMap.end())
		return String::EMPTY;

	return it->second;
}

const String& PackageMetadata::findAssetSourcePath(Asset* asset)
{
	AssetToPathMap::iterator it = mAssetToPathMap.find(asset);
	if (it == mAssetToPathMap.end())
		return String::EMPTY;

	return it->second;
}

Folder* PackageMetadata::newFolder(const String& name, Folder* parent)
{
	if (!parent)
		parent = mRoot;

	return parent->createSubfolder(name);
}

Folder* PackageMetadata::deleteFolder(Folder* folder)
{
	return folder->parent()->deleteSubfolder(folder);
}

void PackageMetadata::renameFolder(Folder* folder, const String& name)
{
	folder->setName(name);
}

const String& PackageMetadata::packageName() 
{
	return mPackageName;
}

void PackageMetadata::renamePackage(const String& name)
{
	mPackageName = name;

	// have to rename the root folder too
	mRoot->setName(name);
}

const String& PackageMetadata::findId(Reflection::Object* object)
{
	ObjectToObjectIdMap::iterator it = mObjectToObjectIdMap.find(object);
	if (it != mObjectToObjectIdMap.end())
		return it->second;

	return String::EMPTY;
}