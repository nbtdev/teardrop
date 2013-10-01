/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "ImportedAsset.h"
#include "Asset/Asset.h"
#include <string.h>

using namespace Teardrop;
using namespace Tools;

ImportedAsset::ImportedAsset()
	: mAsset(0)
	, mMetadata(0)
{

}

ImportedAsset::~ImportedAsset()
{
}

bool ImportedAsset::isValid()
{
	return mAsset != 0;
}

int ImportedAsset::numDependencies()
{
	return int(mDeps.size());
}

Dependencies& ImportedAsset::dependencies()
{
	return mDeps;
}

Asset* ImportedAsset::asset()
{
	return mAsset;
}

Metadata* ImportedAsset::metadata()
{
	return mMetadata;
}

void ImportedAsset::setAsset(Asset* asset, Metadata* metadata)
{
	mAsset = asset;
	mMetadata = metadata;
}

void ImportedAsset::addDep(Asset* asset, const String& name, const String& filepath)
{
	if (asset) {
		Dependency dep;
		dep.mObject = asset;
		dep.mName = name;
		dep.mSourcePath = filepath;
		mDeps.push_back(dep);
	}
}

void ImportedAsset::addDep(Reflection::Object* asset, const String& name)
{
	if (asset) {
		Dependency dep;
		dep.mObject = asset;
		dep.mName = name;
		mDeps.push_back(dep);
	}
}

