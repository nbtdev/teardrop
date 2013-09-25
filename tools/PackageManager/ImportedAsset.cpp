/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "ImportedAsset.h"
#include <string.h>

using namespace Teardrop;
using namespace Tools;

ImportedAsset::ImportedAsset()
	: mAsset(0)
	, mMetadata(0)
	, mDeps(0)
	, mDepsMetadata(0)
	, mDepsFilepath(0)
	, mDepsMetaName(0)
	, mNumDeps(0)
{

}

ImportedAsset::~ImportedAsset()
{
	delete [] mDeps;
	delete [] mDepsMetadata;
	delete [] mDepsFilepath;
	delete [] mDepsMetaName;
}

ImportedAsset::ImportedAsset(const ImportedAsset& other)
	: mAsset(0)
	, mMetadata(0)
	, mDeps(0)
	, mDepsMetadata(0)
	, mDepsFilepath(0)
	, mDepsMetaName(0)
	, mNumDeps(0)
{
	*this = other;
}

ImportedAsset& ImportedAsset::operator=(const ImportedAsset& other)
{
	mAsset = other.mAsset;
	mMetadata = other.mMetadata;
	mNumDeps = other.mNumDeps;

	delete [] mDeps;
	mDeps = 0;

	delete [] mDepsMetadata;
	mDepsMetadata = 0;

	delete [] mDepsFilepath;
	mDepsFilepath = 0;

	delete [] mDepsMetaName;
	mDepsMetaName = 0;

	if (mNumDeps) {
		mDeps = new Asset*[mNumDeps];
		memcpy(mDeps, other.mDeps, sizeof(mDeps[0]) * mNumDeps);

		mDepsMetadata = new Metadata*[mNumDeps];
		memcpy(mDepsMetadata, other.mDepsMetadata, sizeof(mDepsMetadata[0]) * mNumDeps);

		mDepsFilepath = new String[mNumDeps];
		mDepsMetaName = new String[mNumDeps];
		for (int i=0; i<mNumDeps; ++i) {
			mDepsFilepath[i] = other.mDepsFilepath[i];
			mDepsMetaName[i] = other.mDepsMetaName[i];
		}
	}

	return *this;
}

bool ImportedAsset::isValid()
{
	return mAsset != 0;
}

void ImportedAsset::addDep(Asset* asset, const String& filepath, const String& name)
{
	if (asset) {
		int nDeps = mNumDeps++;
		Asset** newDeps = new Asset*[mNumDeps];
		memcpy(newDeps, mDeps, sizeof(mDeps[0]) * nDeps);
		newDeps[nDeps] = asset;
		delete [] mDeps;
		mDeps = newDeps;

		// and resize the deps metadata array too
		delete [] mDepsMetadata;
		mDepsMetadata = new Metadata*[mNumDeps];

		// and the filepaths
		String* newPaths = new String[mNumDeps];
		for (int i=0; i<nDeps; ++i) {
			newPaths[i] = mDepsFilepath[i];
		}
		newPaths[nDeps] = filepath;
		delete [] mDepsFilepath;
		mDepsFilepath = newPaths;

		// and the dependency metadata names
		String* newNames = new String[mNumDeps];
		for (int i=0; i<nDeps; ++i) {
			newNames[i] = mDepsMetaName[i];
		}
		newNames[nDeps] = name;
		delete [] mDepsMetaName;
		mDepsMetaName = newNames;
	}
}