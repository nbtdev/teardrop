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

#include "ProjectItem.h"
#include "PackageManager/PackageManager.h"
#include "PackageManager/PackageMetadata.h"
#include "PackageManager/Folder.h"
#include "Reflection/Reflection.h"
#include "Package/Package.h"

using namespace Teardrop;
using namespace Tools;

ProjectItem::ProjectItem()
	: mPackageMgr(0)
	, mFolder(0)
	, mObject(0)
	, mMetadata(0)
	, mEditor(0)
	, mParent(0)
	, mIsPackage(0)
	, mIsActive(0)
{
}

ProjectItem::ProjectItem(PackageManager* packageMgr, Reflection::Object* obj, Metadata* metadata, ProjectItem* parent/* =0 */)
	: mPackageMgr(packageMgr)
	, mFolder(0)
	, mObject(obj)
	, mMetadata(metadata)
	, mEditor(0)
	, mParent(parent)
	, mIsPackage(0)
	, mIsActive(0)
{
	updatePath();
}

ProjectItem::ProjectItem(PackageManager* packageMgr, ProjectItem* parent)
	: mPackageMgr(packageMgr)
	, mFolder(0)
	, mObject(0)
	, mMetadata(0)
	, mEditor(0)
	, mParent(parent)
	, mIsPackage(1)
	, mIsActive(0)
{
	if (packageMgr) {
		mMetadata = packageMgr->metadata();
		mFolder = packageMgr->metadata()->rootFolder();
	}
}

ProjectItem::ProjectItem(PackageManager* packageMgr, Folder* folder, ProjectItem* parent/* =0 */)
	: mPackageMgr(packageMgr)
	, mFolder(folder)
	, mObject(0)
	, mMetadata(0)
	, mEditor(0)
	, mParent(parent)
	, mIsPackage(0)
	, mIsActive(0)
{
}

ProjectItem::~ProjectItem()
{
	qDeleteAll(mChildren);
}

bool ProjectItem::isRoot() const
{
	// all other items must have mPackageMgr set
	return (mPackageMgr==0);
}

bool ProjectItem::isPackage() const
{
	return mIsPackage != 0;
}

bool ProjectItem::isFolder() const
{
	return (mFolder!=0);
}

bool ProjectItem::isObject() const
{
	return (mObject!=0);
}

bool ProjectItem::isExecutable() const
{
    return (mPackageMgr && (mPackageMgr->executable() != nullptr));
}

ProjectItem* ProjectItem::parent() const
{
	return mParent;
}

ProjectItem* ProjectItem::child(int row) const
{
	return mChildren.at(row);
}

QObject* ProjectItem::editor() const
{
	return mEditor;
}

void ProjectItem::setEditor(QObject* editor)
{
	assert(!mEditor);
	mEditor = editor;
}

int ProjectItem::numChildren() const
{
	return mChildren.size();
}

int ProjectItem::row() const
{
	if (mParent)
		return mParent->mChildren.indexOf(const_cast<ProjectItem*>(this));

	return 0;
}

Reflection::Object* ProjectItem::object() const
{
    return mObject;
}

Executable* ProjectItem::executable() const
{
    if (!mPackageMgr) {
        return nullptr;
    }

    return mPackageMgr->executable();
}

Folder* ProjectItem::folder() const
{
	return mFolder;
}

PackageManager* ProjectItem::packageManager() const
{
	return mPackageMgr;
}

Metadata* ProjectItem::metadata() const
{
	return mMetadata;
}

QString ProjectItem::toString() const
{
	String sVal;
	if (mFolder) {
		sVal = mFolder->name();
	}
	else if (mMetadata) {
		sVal = mMetadata->getName();
	}

	return QString(sVal);
}

void ProjectItem::append(ProjectItem* child)
{
	mChildren.append(child);
	child->setParent(this);
}

void ProjectItem::remove(ProjectItem* child)
{
	mChildren.removeOne(child);
	child->setParent(0);
}

const String& ProjectItem::path() const
{
	return mPath;
}

static void makePath(String& path, Folder* folder)
{
	if (folder) {
		makePath(path, folder->parent());

		path.append("/");
		path.append(folder->name());
	}
}

void ProjectItem::updatePath()
{
	// only bother if it's an object; folder paths aren't displayed
	if (mObject) {
		mPath.clear();
		makePath(mPath, mParent->folder());
		mPath.append("/");
		mPath.append(mMetadata->getName());
	}

	// update the paths of all children (if any)
	for (int i=0; i<mChildren.size(); ++i) {
		mChildren[i]->updatePath();
	}
}

void ProjectItem::setParent(ProjectItem* newParent)
{
	mParent = newParent;

	if (mParent) {
		// update our path
		updatePath();
	}
}

QString ProjectItem::tooltip() const
{
	return toString();
}

ProjectItemData::ProjectItemData(ProjectItem* item)
	: mItem(item)
{

}

ProjectItemData::~ProjectItemData()
{

}

ProjectItem* ProjectItemData::item() const
{
	return mItem;
}

void ProjectItem::rename(const QString& newName)
{
	String str(newName.toLatin1().data());
	if (isFolder())
		mPackageMgr->metadata()->renameFolder(mFolder, str);
	else
		mMetadata->setName(str);
}

void ProjectItem::removeAllChildren()
{
	mChildren.clear();
}

void ProjectItem::setActive(bool active)
{
	mIsActive = active ? 1 : 0;
}

bool ProjectItem::active() const
{
	return mIsActive != 0;
}
