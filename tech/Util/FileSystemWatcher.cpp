/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "win32/FileSystemWatcherWin32.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
FileSystemWatcher* FileSystemWatcher::create(
	const String& watchPath, bool bRecursive, float timeout)
{
	// return the platform-specific implementation...
	return TD_NEW FileSystemWatcherWin32(
		watchPath, bRecursive, timeout);
}
//---------------------------------------------------------------------------
void FileSystemWatcher::destroy(FileSystemWatcher* pWatcher)
{
	delete pWatcher;
}
//---------------------------------------------------------------------------
FileSystemWatcher::~FileSystemWatcher()
{
}
