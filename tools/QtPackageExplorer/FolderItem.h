/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(FOLDERITEM_INCLUDED)
#define FOLDERITEM_INCLUDED

#include "QtPackageExplorer/PackageExplorerItem.h"
#include <list>

namespace Teardrop
{
	class String;

	namespace Reflection
	{
		class Object;
	}

	namespace Tools
	{
		class Folder;
		class FolderItem;
		class ObjectItem;
		class PackageManager;

		class FolderItem : public PackageExplorerItem
		{
		public:
			FolderItem(FolderItem* parent, Folder* folder, PackageManager* pkgMgr);
			~FolderItem();

			// PackageExplorerItemType implementation
			PackageExplorerItem::Type itemType();

			int numFolders() const;
			FolderItem* folder(int idx);
			FolderItem* addFolder(Folder* folder);
			void removeFolder(FolderItem* folder);

			int numObjects() const;
			ObjectItem* object(int idx);
			ObjectItem* addObject(Reflection::Object* object);
			void removeObject(ObjectItem* object);

			FolderItem* parent();
			Folder* folder();

			PackageManager* packageManager();

			// Folder callback
			void onNameChanged(const char* newName);
			void labelChanged(const String& newLabel);

		protected:
			typedef std::list<FolderItem*> FolderItems;
			FolderItems mFolders;

			typedef std::list<ObjectItem*> ObjectItems;
			ObjectItems mObjects;

			FolderItem* mParent;
			Folder* mFolder;

			PackageManager* mPkgMgr;

			bool mChangingName;
		};
	}
}

#endif // FOLDERITEM_INCLUDED
