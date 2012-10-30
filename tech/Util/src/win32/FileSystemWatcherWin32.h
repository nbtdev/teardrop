/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

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
-----------------------------------------------------------------------------
*/

#if !defined(FILESYSTEMWATCHERWIN32_INCLUDED)
#define FILESYSTEMWATCHERWIN32_INCLUDED

#include "Util/include/FileSystemWatcher.h"
#include <map>

namespace CoS
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

		COS_DECLARE_ALLOCATOR();
	};
}

#endif // FILESYSTEMWATCHERWIN32_INCLUDED
