/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Package.h"

using namespace Teardrop;

Package::Package(const String& name)
	: mName(name)
	, mRoot(name)
{
}

Package::~Package()
{
}

const Folder& Package::root() const
{
	return mRoot;
}

Folder& Package::root()
{
	return mRoot;
}

const String& Package::name() const
{
	return mName;
}

void Package::setName(const String& name)
{
	mName = name;
	mRoot.setName(name);
}

Folder::Folder(const String& name, Folder* parent)
	: mName(name)
	, mParent(parent)
{

}

Folder::~Folder()
{

}

bool Folder::add(Reflection::Object* obj)
{
	mObjects.push_back(obj);
	return true;
}

Folder* Folder::newFolder(const String& name)
{
	Folder* rtn = new Folder(name, this);
	mFolders.push_back(rtn);
	return rtn;
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
}

const String& Folder::name() const
{
	return mName;
}
