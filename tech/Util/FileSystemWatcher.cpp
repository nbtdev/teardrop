/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "FileSystemWatcher.h"

using namespace Teardrop;

#if defined(_WIN32) || defined(_WIN64)
#include "win32/FileSystemWatcherWin32.h"

FileSystemWatcher* FileSystemWatcher::create(
	const String& watchPath, bool bRecursive, float timeout)
{
	// return the platform-specific implementation...
	return TD_NEW FileSystemWatcherWin32(
		watchPath, bRecursive, timeout);
}
#else // _WIN32, _WIN64

// TODO: implement POSIX filesystem watcher

FileSystemWatcher* FileSystemWatcher::create(
    const String& /*watchPath*/, bool /*bRecursive*/, float /*timeout*/)
{
    return nullptr;
}

#endif // _WIN32, _WIN64

//---------------------------------------------------------------------------
void FileSystemWatcher::destroy(FileSystemWatcher* pWatcher)
{
	delete pWatcher;
}
//---------------------------------------------------------------------------
FileSystemWatcher::~FileSystemWatcher()
{
}
