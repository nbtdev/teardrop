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
#include "Asset/LandscapeAsset.h"
#include "Asset/StaticMeshAsset.h"
#include "AssetImport.h"
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

void PackageManager::importAsset(ImportedAsset& imp, Folder* folder, const char* filepath, const Reflection::ClassDef* assetClass)
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
			imp.setAsset(texAsset, assetMeta);
		}
	}
	else if (assetClass == StaticMeshAsset::getClassDef()) {
		StaticMeshAsset* asset = importStaticMesh(imp, filepath);
		if (asset) {
			// add the new asset to the package
			mPackage->add(asset);

			// and add an entry to the package metadata
			String assetId;
			Metadata* assetMeta = mMetadata->add(assetId, folder, asset, filepath);
			assetMeta->generateThumbnail();
			imp.setAsset(asset, assetMeta);
		}
	}
	else if (assetClass == LandscapeAsset::getClassDef()) {
		// TODO: support additional landscape editors?
		LandscapeAsset* landscapeAsset = importLandscape(imp, filepath, LANDSCAPEASSET_TYPE_L3DT);
		if (landscapeAsset) {
			// add the new asset to the package
			mPackage->add(landscapeAsset);

			// and add an entry to the package metadata
			String assetId;
			Metadata* assetMeta = mMetadata->add(assetId, folder, landscapeAsset, filepath);
			assetMeta->generateThumbnail();

			imp.setAsset(landscapeAsset, assetMeta);

			// add all dependent assets to this folder too
			for (Dependencies::iterator it = imp.dependencies().begin(); it != imp.dependencies().end(); ++it) {
				Dependency& dep = *it;

				mPackage->add(dep.mObject);

				// and the metadata...
				String assetId;
				Metadata* depMeta = mMetadata->add(assetId, folder, static_cast<Asset*>(dep.mObject), dep.mSourcePath);
				dep.mMetadata = depMeta;
				depMeta->setName(dep.mName);
				depMeta->generateThumbnail();
			}
		}
	}
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

void PackageManager::remove(Reflection::Object* obj)
{
	if (!obj)
		return;

	if (mMetadata)
		mMetadata->remove(obj);

	// and then finally, from the package
	mPackage->remove(obj);
}

void PackageManager::remove(Folder* folder, bool bRecursive/* =false */)
{
	if (!folder)
		return;

	// metadata has to be responsible for deleting objects from the package, since
	// only it knows whether objects are deleted, or transferred, and if so, to where 
	if (mMetadata)
		mMetadata->deleteFolder(folder, bRecursive);
}