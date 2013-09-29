/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(QTPROJECTITEM_INCLUDED)
#define QTPROJECTITEM_INCLUDED

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

		class QtProjectItem
		{
		public:
			// empty package item
			QtProjectItem();

			// top-level package item
			QtProjectItem(PackageManager* packageMgr, QtProjectItem* parent);

			// child folder and object items
			QtProjectItem(PackageManager* packageMgr, Reflection::Object* obj, Metadata* metadata, QtProjectItem* parent);
			QtProjectItem(PackageManager* packageMgr, Folder* folder, QtProjectItem* parent);

			~QtProjectItem();

			Reflection::Object* object() const;
			Folder* folder() const;
			Metadata* metadata() const;
			PackageManager* packageManager() const;
			QString toString() const;
			QString tooltip() const;
			QtProjectItem* child(int row) const;
			QtProjectItem* parent() const;
			int row() const;
			int numChildren() const;
			void append(QtProjectItem* child);
			void remove(QtProjectItem* child);
			void removeAllChildren();
			void rename(const QString& newName);
			const String& path() const;

			// true if this item is the root item in a model
			bool isRoot() const;
			// true if this item is a Package (top-level items only)
			bool isPackage() const;
			// return true if this item represents a folder
			bool isFolder() const;
			// return true if this item represents a Reflection::Object
			bool isObject() const;

		protected:
			void setParent(QtProjectItem* newParent);
			void updatePath();

		private:
			PackageManager* mPackageMgr;
			Folder* mFolder;
			Reflection::Object* mObject;
			Metadata* mMetadata;

			QList<QtProjectItem*> mChildren;
			QtProjectItem* mParent;
			String mPath;

			int mIsPackage;
		};

		// QObjectUserData wrapper for QtProjectItem
		class QtProjectItemData : public QObjectUserData
		{
		public:
			QtProjectItemData(QtProjectItem* item);
			~QtProjectItemData();
			QtProjectItem* item() const;

		protected:
			QtProjectItem* mItem;
		};
	}
}

#endif // QTPROJECTITEM_INCLUDED
