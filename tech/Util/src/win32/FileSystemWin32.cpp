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

#include "stdafx.h"
#include "FileSystem.h"

using namespace CoS;
//---------------------------------------------------------------------------
void FileSystem::glob(
	const String& dirName, 
	const String& searchPattern, 
	FileList& files, 
	bool recursive/* =false */)
{
	files.clear();

	String findSpec(dirName);
	findSpec += "\\";
	findSpec += searchPattern;

	WIN32_FIND_DATA data;
	HANDLE hFile = FindFirstFile(findSpec, &data);
	BOOL found = (hFile != INVALID_HANDLE_VALUE);

	while (found)
	{
		String pathName(dirName);
		pathName += "\\";
		pathName += data.cFileName;

		// if this is a directory, and isn't . or .., and we are recursing, then
		// recurse into this directory
		if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
		{
			if (String(".") != data.cFileName && 
				String("..") != data.cFileName &&
				String(".svn") != data.cFileName)
			{
				if (recursive)
					glob(pathName, searchPattern, files, recursive);
			}
		}

		// otherwise, add it to the list
		files.push_back(pathName);

		// and find the next
		found = FindNextFile(hFile, &data);
	}

	FindClose(hFile);
}
//---------------------------------------------------------------------------
void FileSystem::findAll(
	const String& dirName, 
	const String& fileName, 
	FileList& files, 
	bool recursive/* =false */)
{
	String globSpec(dirName);
	globSpec += "\\*";

	// first, find everything in the search root
	WIN32_FIND_DATA globData;
	HANDLE hGlob = FindFirstFile(globSpec, &globData);
	BOOL found = (hGlob != INVALID_HANDLE_VALUE);

	while (found)
	{
		if ((globData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
		{
			// ignore "." and ".."
			if (String(".") != globData.cFileName && 
				String("..") != globData.cFileName &&
				String(".svn") != globData.cFileName)
			{
				// if we're recursive, then recurse
				if (recursive)
				{
					String newGlobDir(dirName);
					newGlobDir += "\\";
					newGlobDir += globData.cFileName;
					findAll(newGlobDir, fileName, files, recursive);
				}
			}
		}

		found = FindNextFile(hGlob, &globData);
	}

	FindClose(hGlob);

	// then, look in this directory for our target file
	String fileSpec(dirName);
	fileSpec += "\\";
	fileSpec += fileName;
	WIN32_FIND_DATA fileData;
	HANDLE hFile = FindFirstFile(fileSpec, &fileData);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		files.push_back(fileSpec);
	}
	FindClose(hFile);
}
//---------------------------------------------------------------------------
void FileSystem::makeRelativePath(
	String& result, const String& from, const String& to)
{
	char buf[MAX_PATH] = {0};

	PathRelativePathTo(
		buf, 
		from, 
		FILE_ATTRIBUTE_DIRECTORY, 
		to, 
		FILE_ATTRIBUTE_NORMAL);

	result = buf;
}
//---------------------------------------------------------------------------
void FileSystem::getAppDataPath(String& path)
{
	path.clear();

	// get "special" local appdata folder
	char buf[MAX_PATH];
	SHGetFolderPath(
					NULL, 
					CSIDL_LOCAL_APPDATA|CSIDL_FLAG_CREATE, 
					NULL, 
					0, 
					buf);

	path = buf;
	path += "\\";
	path += "CoS";
}
