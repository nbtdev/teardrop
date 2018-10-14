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
