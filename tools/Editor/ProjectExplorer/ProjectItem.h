/******************************************************************************
Copyright (c) 2015 Teardrop Games

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

#if !defined(PROJECTITEM_INCLUDED)
#define PROJECTITEM_INCLUDED

#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QObject>
#include "Util/_String.h"

namespace Teardrop 
{
	class Package;

	namespace Reflection
	{
		class Object;
	}

	namespace Tools 
	{
		class Project;
		class Folder;
		class Metadata;
		class PackageManager;

		class ProjectItem
		{
		public:
			// empty package item
			ProjectItem();

			// top-level package item
			ProjectItem(PackageManager* packageMgr, ProjectItem* parent);

			// child folder and object items
			ProjectItem(PackageManager* packageMgr, Reflection::Object* obj, Metadata* metadata, ProjectItem* parent);
			ProjectItem(PackageManager* packageMgr, Folder* folder, ProjectItem* parent);

			~ProjectItem();

			Reflection::Object* object() const;
			Folder* folder() const;
			Metadata* metadata() const;
			PackageManager* packageManager() const;
			QString toString() const;
			QString tooltip() const;
			ProjectItem* child(int row) const;
			ProjectItem* parent() const;
			QObject* editor() const;
			void setEditor(QObject* editor);
			int row() const;
			int numChildren() const;
			void append(ProjectItem* child);
			void remove(ProjectItem* child);
			void removeAllChildren();
			void rename(const QString& newName);
			const String& path() const;
			void setActive(bool active);
			bool active() const;

			// true if this item is the root item in a model
			bool isRoot() const;
			// true if this item is a Package (top-level items only)
			bool isPackage() const;
			// return true if this item represents a folder
			bool isFolder() const;
			// return true if this item represents a Reflection::Object
			bool isObject() const;

		protected:
			void setParent(ProjectItem* newParent);
			void updatePath();

		private:
			PackageManager* mPackageMgr;
			Folder* mFolder;
			Reflection::Object* mObject;
			Metadata* mMetadata;
			QObject* mEditor;

			QList<ProjectItem*> mChildren;
			ProjectItem* mParent;
			String mPath;

			int mIsPackage;
			int mIsActive;
		};

		// QObjectUserData wrapper for ProjectItem
		class ProjectItemData : public QObjectUserData
		{
		public:
			ProjectItemData(ProjectItem* item);
			~ProjectItemData();
			ProjectItem* item() const;

		protected:
			ProjectItem* mItem;
		};
	}
}

#endif // PROJECTITEM_INCLUDED
