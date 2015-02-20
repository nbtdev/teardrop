/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "FileSystem.h"
#include "_String.h"

using namespace Teardrop;

void FileSystem::directoryName(String& dirName, const String& pathname)
{
	dirName.clear();
	size_t pos = pathname.findLast('\\');

	if (pos == String::INVALID)
		pos = pathname.findLast('/');

	if (pos != String::INVALID) {
		dirName = pathname.substr(0, pos);
	}

	// go through and make all path seps to be / type
	for (size_t i=0; i<dirName.length(); ++i) {
		if (dirName[i] == '\\') 
			dirName[i] = '/';
	}
}

void FileSystem::fileName(String& fileName, const String& pathname)
{
	fileName.clear();
	size_t pos = pathname.findLast('\\');

	if (pos == String::INVALID)
		pos = pathname.findLast('/');

	if (pos != String::INVALID) {
		fileName = pathname.substr(pos+1);
	}
}

void FileSystem::baseName(String& baseName, const String& pathname)
{
	fileName(baseName, pathname);

	// then strip off anything after (and including) the final '.'
	size_t pos = baseName.findLast('.');

	if (pos != String::INVALID) {
		String tmp = baseName.substr(0, pos);
		baseName = tmp;
	}
}
