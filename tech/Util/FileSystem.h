/******************************************************************************
Copyright (c) 2018 Teardrop Games

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
******************************************************************************/

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

		// path parts utility functions
		static void fileName(/*out*/String& fileName, /*in*/const String& pathname);
		static void directoryName(/*out*/String& dirName, /*in*/const String& pathname);
		static void baseName(/*out*/String& baseName, /*in*/const String& pathname);

		// returns true if the two paths are the same, false otherwise
		static bool isSamePath(const String& lhs, const String& rhs);

		// copy file from 'from' to 'to', returns false if failed for any reason; set 'overwrite' to 'true'
		// to overwrite existing 'to' file
		static bool copyFile(const String& from, const String& to, bool overwrite=false);

		// thanks Microsoft...
		static const char PATHSEP;
	};
}

#endif // FILESYSTEM_INCLUDED
