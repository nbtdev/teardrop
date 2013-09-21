/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "PackageManager.h"
#include "PackageMetadata.h"
#include "Folder.h"
#include "Package/Package.h"
#include "Package/PackageSerializer.h"
#include "Asset/TextureAsset.h"
#include "Stream/FileStream.h"
#include <utility>

using namespace Teardrop;
using namespace Tools;

PackageManager::PackageManager()
	: mPackage(0)
	, mMetadata(0)
{
	mPackage = new Package;
	mMetadata = new PackageMetadata(mPackage);
}

PackageManager::~PackageManager()
{
	delete mMetadata;
	delete mPackage;
}

std::pair<Asset*, Metadata*> PackageManager::importAsset(Folder* folder, const char* filepath, const Reflection::ClassDef* assetClass)
{
	// TODO: get these known types from some lookup table?
	if (assetClass == TextureAsset::getClassDef()) {
		TextureAsset* texAsset = importTexture(filepath, TEXTUREASSET_TYPE_BCX);
		if (texAsset) {
			// add the new asset to the package
			mPackage->add(texAsset);

			// and add an entry to the package metadata
			String assetId;
			Metadata* assetMeta = mMetadata->add(assetId, folder, texAsset, filepath);
			assetMeta->generateThumbnail();
			return std::pair<Asset*, Metadata*>(texAsset, assetMeta);
		}
	}

	return std::pair<Asset*, Metadata*>(0,0);
}

std::pair<Reflection::Object*, Metadata*> PackageManager::createObject(Folder* folder, const Reflection::ClassDef* classDef)
{
	// create the object
	Reflection::Object* obj = classDef->createInstance();
	mPackage->add(obj);
	String uuid;
	Metadata* meta = mMetadata->add(uuid, folder, obj);
	return std::pair<Reflection::Object*, Metadata*>(obj, meta);
}

PackageMetadata* PackageManager::metadata()
{
	return mMetadata;
}

String PackageManager::packageFilename()
{
	String name(metadata()->getName());
	name.replaceAll(' ', '_');
	name += ".package";
	return name;
}

bool PackageManager::save(const String& path)
{
	String packagePathName(path);
	packagePathName += packageFilename();

	FileStream strm;
	if (!strm.open(packagePathName, WRITE|TRUNCATE))
		return false;

	PackageSerializer ser(mPackage);
	return ser.serialize(strm, mMetadata);
}

bool PackageManager::load(const String& path, DeferredObjectResolves& deferred, ObjectIdToObject& lut)
{
	String packagePathName(path);
	packagePathName += packageFilename();

	FileStream strm;
	if (!strm.open(packagePathName, READ))
		return false;

	PackageSerializer ser(mPackage);
	bool rtn = ser.deserialize(strm, deferred, lut, mMetadata);

	return rtn;
}

void PackageManager::getAllMetadata(std::list<Metadata*>& metaList) const
{
	if (mMetadata)
		mMetadata->getAllMetadata(metaList);
}