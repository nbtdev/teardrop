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

#if !defined(FILESYSTEMWATCHER_INCLUDED)
#define FILESYSTEMWATCHER_INCLUDED

#include "Util/include/_String.h"
#include "Memory/include/Allocators.h"
#include <list>

namespace CoS
{
	class String;

	class FileSystemWatcher
	{
	public:
		// provide the path to watch
		static FileSystemWatcher* create(
			const String& watchPath,	// pathname to watch
			bool bRecursive	= true,		// watch or don't watch subtree
			float timeout = 0.5f		// how much time until we decide that a file is "settled down"
			);
		static void destroy(FileSystemWatcher* pWatcher);
		virtual ~FileSystemWatcher();

		// rather than spawn a separate thread for each watcher, simply 
		// poll for changes at the owner's leisure
		typedef std::list<String> FileSystemChanges;
		virtual void update(FileSystemChanges& changed) = 0;

		COS_DECLARE_ALLOCATOR();
	};
}

#endif // FILESYSTEMWATCHER_INCLUDED
