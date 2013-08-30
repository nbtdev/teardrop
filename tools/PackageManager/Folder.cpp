/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Folder.h"
#include <algorithm>

using namespace Teardrop;
using namespace Tools;

Folder::Folder(const String& name, Folder* parent)
	: mParent(parent)
	, mName(name)
{
}

Folder::~Folder()
{
	for (Folders::iterator it = mFolders.begin(); it != mFolders.end(); ++it) {
		delete *it;
	}
}

void Folder::add(Reflection::Object* obj)
{
	mObjects.push_back(obj);
}

void Folder::remove(Reflection::Object* obj)
{
	mObjects.remove(obj);
}

const Folders& Folder::folders() const
{
	return mFolders;
}

const Objects& Folder::objects() const
{
	return mObjects;
}

void Folder::setName(const String& name)
{
	mName = name;
	if (NameChanged)
		NameChanged(name);
}

const String& Folder::name() const
{
	return mName;
}

Folder* Folder::parent() const
{
	return mParent;
}

void Folder::add(Folder* subFolder)
{
	mFolders.push_back(subFolder);
}

Folder* Folder::createSubfolder(const String& name)
{
	Folder* newFolder = new Folder(name, this);
	add(newFolder);
	return newFolder;
}

Folder* Folder::deleteSubfolder(Folder* folder)
{
	Folders::iterator it = std::find(mFolders.begin(), mFolders.end(), folder);
	if (it != mFolders.end()) {
		mFolders.erase(it);
		return folder;
	}

	return 0;
}