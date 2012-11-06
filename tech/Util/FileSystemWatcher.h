/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(FILESYSTEMWATCHER_INCLUDED)
#define FILESYSTEMWATCHER_INCLUDED

#include "Util/_String.h"
#include "Memory/Allocators.h"
#include <list>

namespace Teardrop
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

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // FILESYSTEMWATCHER_INCLUDED
