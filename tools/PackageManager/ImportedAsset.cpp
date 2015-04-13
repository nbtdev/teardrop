/******************************************************************************
Copyright (c) 2015 Teardrop Games

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
******************************************************************************/

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

int ImportedAsset::numInternalDependencies()
{
	return int(mInternalDeps.size());
}

Dependencies& ImportedAsset::dependencies()
{
	return mDeps;
}

InternalDependencies& ImportedAsset::internalDependencies()
{
	return mInternalDeps;
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

void ImportedAsset::addInternalDep(Reflection::Object* obj)
{
	if (obj) {
		mInternalDeps.push_back(obj);
	}
}
