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

Asset* PackageManager::importAsset(Folder* folder, const char* filepath, const Reflection::ClassDef* assetClass)
{
	// TODO: get these known types from some lookup table?
	if (assetClass == TextureAsset::getClassDef()) {
		TextureAsset* texAsset = importTexture(filepath, TEXTUREASSET_TYPE_BCX);
		if (texAsset) {
			// add the new asset to the package
			mPackage->add(texAsset);

			// and add an entry to the package metadata
			String assetId;
			mMetadata->add(assetId, folder, texAsset, filepath);
			return texAsset;
		}
	}

	return 0;
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

bool PackageManager::load(const String& path)
{
	String packagePathName(path);
	packagePathName += packageFilename();

	FileStream strm;
	if (!strm.open(packagePathName, READ))
		return false;

	PackageSerializer ser(mPackage);
	return ser.deserialize(strm, mMetadata);
}
