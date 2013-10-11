/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "FolderDragDropData.h"

using namespace Teardrop;
using namespace Tools;

FolderDragDropData::FolderDragDropData(Folder* folder, const String& path, Package* pkg)
	: mFolder(folder)
	, mPath(path)
	, mPackage(pkg)
{
}

FolderDragDropData::~FolderDragDropData()
{
}

DragDropData::Type FolderDragDropData::type()
{
	return DDD_FOLDER;
}

const String& FolderDragDropData::path()
{
	return mPath;
}

Folder* FolderDragDropData::folder()
{
	return mFolder;
}

Package* FolderDragDropData::package()
{
	return mPackage;
}
