/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "PackageMetadata.h"
#include "TextureAssetMetadata.h"
#include "Folder.h"
#include "Uuid.h"
#include "Asset/Package.h"
#include "Asset/TextureAsset.h"

using namespace Teardrop;
using namespace Tools;

TD_CLASS_IMPL(PackageMetadata)

PackageMetadata::PackageMetadata()
	: mRoot(0)
	, mPackage(0)
{
}

PackageMetadata::PackageMetadata(Package* package)
	: mRoot(0)
	, mPackage(package)
{
	PropertyChanged.bind(this, &PackageMetadata::onPropertyChanged);
	mRoot = new Folder(getName(), 0);
}

PackageMetadata::~PackageMetadata()
{

}

Folder* PackageMetadata::rootFolder()
{
	return mRoot;
}

Metadata* PackageMetadata::add(String& uuid, Folder* parent, Asset* asset, const char* assetSourcePath)
{
	// call the general object insert handler first...
	add(uuid, parent, asset);

	Metadata* rtn = 0;

	// make new metadata for the asset
	// TODO: should this be one level higher in the call stack?
	if (asset->getDerivedClassDef() == TextureAsset::getClassDef()) {
		TextureAssetMetadata* meta = new TextureAssetMetadata(static_cast<TextureAsset*>(asset));
		meta->setSourcePath(assetSourcePath);
		meta->setID(uuid);

		mObjectToMetadataMap[asset] = meta;
		rtn = meta;
	}

	// and actually add it to the folder
	parent->add(asset);

	return rtn;
}

void PackageMetadata::add(String& uuid, Folder* parent, Reflection::Object* object)
{
	UuidGen(uuid);

	// map "object <--> ID"
	mObjectIdToObjectMap[uuid] = object;

	parent->add(object);
	mObjectToFolderMap[object] = parent;
}

void PackageMetadata::remove(const String& objectId)
{
	// and then the others
	ObjectIdToObjectMap::iterator it = mObjectIdToObjectMap.find(objectId);
	if (it != mObjectIdToObjectMap.end()) {
		Reflection::Object* obj = it->second;

		// remove from this map 
		mObjectIdToObjectMap.erase(it);

		// "object --> folder" map...
		mObjectToFolderMap.erase(obj);

		// object --> metadata map
		mObjectToMetadataMap.erase(obj);
	}
}

void PackageMetadata::remove(Reflection::Object* object)
{
	// "object --> folder" map...
	mObjectToFolderMap.erase(object);

	ObjectToMetadataMap::iterator it = mObjectToMetadataMap.find(object);
	if (it != mObjectToMetadataMap.end()) {
		// and the "object ID --> object" map
		mObjectIdToObjectMap.erase(it->second->getID());

		// and the metadata
		mObjectToMetadataMap.erase(it);
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
	if (folder == mRoot)
		setName(name); // rename the package too
}

void PackageMetadata::onPropertyChanged(const Reflection::PropertyDef* prop)
{
	if (String("Name") == prop->getName()) {
		// have to rename the root folder too
		mRoot->setName(getName());
	}
}

Metadata* PackageMetadata::findObjectMetadata(Reflection::Object* object)
{
	ObjectToMetadataMap::iterator it = mObjectToMetadataMap.find(object);
	if (it != mObjectToMetadataMap.end())
		return it->second;

	return 0;
}

Reflection::Object* PackageMetadata::findObject(const String& id)
{
	ObjectIdToObjectMap::iterator it = mObjectIdToObjectMap.find(id);
	if (it != mObjectIdToObjectMap.end())
		return it->second;

	return 0;
}
