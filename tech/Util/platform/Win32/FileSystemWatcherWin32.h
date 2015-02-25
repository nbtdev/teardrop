/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(FILESYSTEMWATCHERWIN32_INCLUDED)
#define FILESYSTEMWATCHERWIN32_INCLUDED

#include "Util/FileSystemWatcher.h"
#include <map>

namespace Teardrop
{
	class FileSystemWatcherWin32
		: public FileSystemWatcher
	{
		// I/O completion port handle
		HANDLE m_hIOCP;
		// handle to watched directory
		HANDLE m_hDir;
		// how long are we willing to wait for an active file to settle out
		float m_timeout;
		// whether to watch the directory and all of its contents recursively
		bool m_bRecursive;
		// file-change-data buffer
		static const int BUFFER_SIZE = 64 * 1024;
		char* m_pData;
		// OVERLAPPED struct for asynchronous change notifications
		OVERLAPPED m_ovl;
		// since we can get many changes for a single file change, we want to 
		// allow time for changes to settle down before propagating notifications
		typedef std::map<
			String,	// filename
			float	// time of most recent change
		> ActiveFiles;
		ActiveFiles m_activeFiles;

	public:
		FileSystemWatcherWin32(
			const String& watchPath, bool bRecursive = true, float timeout = 0.5f);
		virtual ~FileSystemWatcherWin32();

		// FileSystemWatcher implementation
		void update(FileSystemChanges& changed);

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // FILESYSTEMWATCHERWIN32_INCLUDED
