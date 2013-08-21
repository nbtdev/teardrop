/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "PackageItem.h"
#include "FolderItem.h"
#include "Asset/Package.h"

using namespace Teardrop;
using namespace Tools;

PackageItem::PackageItem(Package* package)
	: mPackage(package)
{
	mRoot = new FolderItem(this, &package->root());
	//addChild(mRoot);
	setText(0, (const char*)package->name());
}

PackageItem::~PackageItem()
{
	delete mRoot;
}

FolderItem* PackageItem::root()
{
	return mRoot;
}

Package* PackageItem::package()
{
	return mPackage;
}
