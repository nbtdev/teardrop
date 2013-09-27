/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "PackageMetadata.h"
#include "ObjectMetadata.h"
#include "TextureAssetMetadata.h"
#include "LandscapeAssetMetadata.h"
#include "HeightfieldAssetMetadata.h"
#include "AttributeMapAssetMetadata.h"
#include "Folder.h"
#include "Thumbnail.h"
#include "Stream/Stream.h"
#include "Util/UUID.h"
#include "Package/Package.h"
#include "Asset/TextureAsset.h"
#include "Asset/LandscapeAsset.h"
#include "Asset/HeightfieldAsset.h"
#include "Asset/AttributeMapAsset.h"
#include "tinyxml/tinyxml.h"
#include <tbb/task.h>

using namespace Teardrop;
using namespace Tools;

TD_CLASS_IMPL(PackageMetadata)

PackageMetadata::PackageMetadata()
	: mRoot(0)
	, mPackage(0)
{
	mRoot = new Folder(getName(), 0);
}

PackageMetadata::PackageMetadata(Package* package)
	: mRoot(0)
	, mPackage(package)
{
	PropertyChanged.bind(fastdelegate::MakeDelegate(this, &PackageMetadata::onPropertyChanged));
	mRoot = new Folder(getName(), 0);
}

PackageMetadata::~PackageMetadata()
{
	PropertyChanged.unbind(fastdelegate::MakeDelegate(this, &PackageMetadata::onPropertyChanged));

	for (ObjectToMetadataMap::iterator it = mObjectToMetadataMap.begin(); it != mObjectToMetadataMap.end(); ++it) {
		delete it->second;
	}
}

Folder* PackageMetadata::rootFolder()
{
	return mRoot;
}

Metadata* PackageMetadata::add(String& uuid, Folder* parent, Asset* asset, const char* assetSourcePath)
{
	// call the general object insert handler first...
	add(uuid, parent, asset);

	AssetMetadata* meta = 0;

	// make new metadata for the asset
	// TODO: should this be one level higher in the call stack?
	if (asset->getDerivedClassDef() == TextureAsset::getClassDef()) {
		meta = new TextureAssetMetadata(static_cast<TextureAsset*>(asset));
	}
	else if(asset->getDerivedClassDef() == LandscapeAsset::getClassDef()) {
		meta = new LandscapeAssetMetadata(static_cast<LandscapeAsset*>(asset));
	}
	else if(asset->getDerivedClassDef() == HeightfieldAsset::getClassDef()) {
		meta = new HeightfieldAssetMetadata(static_cast<HeightfieldAsset*>(asset));
	}
	else if(asset->getDerivedClassDef() == AttributeMapAsset::getClassDef()) {
		meta = new AttributeMapAssetMetadata(static_cast<AttributeMapAsset*>(asset));
	}

	if (meta) {
		meta->setSourcePath(assetSourcePath);
		meta->setID(uuid);

		mObjectToMetadataMap[asset] = meta;
	}

	return meta;
}

Metadata* PackageMetadata::add(String& uuid, Folder* parent, Reflection::Object* object)
{
	UUID newUUID;
	newUUID.generate();
	newUUID.toString(uuid);
	object->setObjectId(newUUID);

	// map "object <--> ID"
	mObjectIdToObjectMap[uuid] = object;

	parent->add(object);
	mObjectToFolderMap[object] = parent;

	ObjectMetadata* meta = new ObjectMetadata(object);
	String id;
	object->getObjectId().toString(id);
	meta->setID(id);

	mObjectToMetadataMap[object] = meta;
	return meta;
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

void PackageMetadata::move(Folder* folder, Folder* newParent)
{
	if (!newParent)
		newParent = mRoot;

	if (!folder || !newParent)
		return;

	const Folders& folders = newParent->folders();
	Folders::const_iterator it = std::find(folders.begin(), folders.end(), folder);
	if (it != folders.end())
		return;

	folder->parent()->remove(folder);
	newParent->add(folder);
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


static void addMetadata(TiXmlElement& objElem, Metadata* meta)
{
	Reflection::ClassDef* classDef = meta->getDerivedClassDef();
	TiXmlElement metadata("metadata");
	metadata.SetAttribute("class", classDef->getName());

	bool bHasMetadata = false;

	while (classDef) {
		const Reflection::PropertyDef* prop = classDef->getProps();
		while (prop) {
			TiXmlElement property("property");
			property.SetAttribute("name", prop->getName());

			String sVal;
			prop->getDataAsString(meta, sVal);
			property.SetAttribute("value", sVal);

			metadata.InsertEndChild(property);
			bHasMetadata = true;
			prop = prop->m_pNext;
		}
		classDef = classDef->getBaseClass();
	}

	if (bHasMetadata)
		objElem.InsertEndChild(metadata);
}

static void addFolders(TiXmlElement& parentElem, PackageMetadata* meta, Folder* folder)
{
	const Folders& folders = folder->folders();
	for (Folders::const_iterator it = folders.begin(); it != folders.end(); ++it) {
		TiXmlElement folderElem("folder");
		folderElem.SetAttribute("name", (*it)->name());
		addFolders(folderElem, meta, *it);
		parentElem.InsertEndChild(folderElem);
	}

	// then add the objects in this folder -- simple, just use IDs
	const Tools::Objects& objs = folder->objects();
	for (Tools::Objects::const_iterator it = objs.begin(); it != objs.end(); ++it) {
		Reflection::Object* obj = *it;
		String objId;
		obj->getObjectId().toString(objId);
		TiXmlElement objElem("object");
		objElem.SetAttribute("id", objId);

		Metadata* metadata = meta->findObjectMetadata(obj);
		if (metadata) {
			addMetadata(objElem, metadata);
		}

		parentElem.InsertEndChild(objElem);
	}
}

void PackageMetadata::serialize(Package* pkg, Stream& strm)
{
	// then serialize folder structure to XML
	TiXmlDocument doc;
	TiXmlElement folders("folders");
	addFolders(folders, this, mRoot);
	TiXmlPrinter printer;
	doc.InsertEndChild(folders);
	doc.Accept(&printer);
	String xml(printer.CStr());

	int len = xml.length()+1;
	strm.write(&len, sizeof(len));
	strm.write((const char*)xml, len);
}

void PackageMetadata::loadFolders(TiXmlElement* elem, Folder* parent, Package* pkg)
{
	TiXmlElement* folder = elem->FirstChildElement("folder");
	while (folder) {
		const char* folderName = folder->Attribute("name");
		Folder* subFolder = parent->createSubfolder(folderName);
		loadFolders(folder, subFolder, pkg);
		folder = folder->NextSiblingElement("folder");
	}

	TiXmlElement* object = elem->FirstChildElement("object");
	while (object) {
		const char* id = object->Attribute("id");
		
		Reflection::Object* obj = 0;
		if (id)
			obj = pkg->findById(id);

		assert(obj);

		TiXmlElement* metaElem = object->FirstChildElement("metadata");
		if (metaElem) {
			const char* className = metaElem->Attribute("class");

			if (className) {
				// create a metadata object of the specified type, and populate its properties
				Reflection::ClassDef* classDef = Reflection::ClassDef::findClassDef(className);
				if (classDef) {
					Reflection::Object* metadata = classDef->createInstance();
					if (metadata) {
						metadata->setupPropertyDefaults();

						TiXmlElement* propElem = metaElem->FirstChildElement("property");
						while (propElem) {
							const char* name = propElem->Attribute("name");
							const char* value = propElem->Attribute("value");

							if (name && value) {
								const Reflection::PropertyDef* prop = classDef->findProperty(name, true);
								if (prop && obj) {
									prop->setDataFromString(metadata, value);
								}
							}

							propElem = propElem->NextSiblingElement("property");
						}

						mObjectToMetadataMap[obj] = static_cast<Metadata*>(metadata);
					}
				}
			}
		}

		mObjectToFolderMap[obj] = parent;
		parent->add(obj);

		String objId;
		obj->getObjectId().toString(objId);
		mObjectIdToObjectMap[objId] = obj;

		object = object->NextSiblingElement("object");
	}
}

void PackageMetadata::deserialize(Package* pkg, Stream& strm)
{
	int len;
	strm.read(&len, sizeof(len));
	char* buf = new char[len];
	strm.read(buf, len);

	TiXmlDocument doc;
	doc.Parse(buf);
	if (doc.Error())
		return;

	TiXmlElement* root = doc.RootElement();
	mRoot = new Folder(getName(), 0);
	loadFolders(root, mRoot, pkg);
}

void PackageMetadata::getAllMetadata(std::list<Metadata*>& metaList) const
{
	for (ObjectToMetadataMap::const_iterator it = mObjectToMetadataMap.begin(); it != mObjectToMetadataMap.end(); ++it) {
		metaList.push_back(it->second);
	}
}