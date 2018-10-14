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

#include "Folder.h"
#include <algorithm>

using namespace Teardrop;
using namespace Tools;

Folder::Folder(const String& name, Folder* parent)
	: mParent(parent)
	, mName(name)
{
}

Folder::~Folder()
{
	for (Folders::iterator it = mFolders.begin(); it != mFolders.end(); ++it) {
		delete *it;
	}
}

void Folder::add(Reflection::Object* obj)
{
	mObjects.push_back(obj);
}

void Folder::remove(Reflection::Object* obj)
{
	mObjects.remove(obj);
}

void Folder::remove(Folder* folder)
{
	// todo: what to do with this folder's children?
	mFolders.remove(folder);
}

const Folders& Folder::folders() const
{
	return mFolders;
}

const Objects& Folder::objects() const
{
	return mObjects;
}

void Folder::setName(const String& name)
{
	mName = name;
    NameChanged.raise(name);
}

const String& Folder::name() const
{
	return mName;
}

Folder* Folder::parent() const
{
	return mParent;
}

void Folder::add(Folder* subFolder)
{
	mFolders.push_back(subFolder);
}

Folder* Folder::createSubfolder(const String& name)
{
	Folder* newFolder = new Folder(name, this);
	add(newFolder);
	return newFolder;
}

Folder* Folder::removeSubfolder(Folder* folder)
{
	Folders::iterator it = std::find(mFolders.begin(), mFolders.end(), folder);
	if (it != mFolders.end()) {
		mFolders.erase(it);
		return folder;
	}

	return 0;
}
