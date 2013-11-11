/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PROJECTITEM_INCLUDED)
#define PROJECTITEM_INCLUDED

#include <QString>
#include <QList>
#include <QObject>
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
