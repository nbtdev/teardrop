/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(FILESYSTEM_INCLUDED)
#define FILESYSTEM_INCLUDED

#include "Util/_String.h"
#include <list>

namespace Teardrop
{
	class FileSystem
	{
	public:
		typedef std::list<String> FileList;
		static void glob(const String& startingDirectory, const String& searchPattern, FileList& files, bool recursive=false);
		static void findAll(const String& startingDirectory, const String& fileName, FileList& files, bool recursive=false);

		// "to" is the full pathname; "from" is the partial directory "root" for the relative path operation
		static void makeRelativePath(/*out*/String& result, /*in*/const String& from, /*in*/const String& to);

		// get the path to the "appdata" directoy
		static void getAppDataPath(/*out*/String& path);

		// make path, creating intermediate directories if they don't exist; returns false if creation failed
		static bool createDirectory(const String& dirPath);

		// thanks Microsoft...
		static const char PATHSEP;
	};
}

#endif // FILESYSTEM_INCLUDED
