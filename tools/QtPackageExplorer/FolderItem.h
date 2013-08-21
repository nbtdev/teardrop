/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(FOLDERITEM_INCLUDED)
#define FOLDERITEM_INCLUDED

#include <QTreeWidgetItem>
#include <list>

namespace Teardrop
{
	class Folder;

	namespace Tools
	{
		class PackageItem;
		class FolderItem;
		class ObjectItem;

		class FolderItem : public QTreeWidgetItem
		{
		public:
			FolderItem(PackageItem* owner, Folder* folder);
			FolderItem(FolderItem* parent, Folder* folder);
			~FolderItem();

			int numFolders() const;
			FolderItem* folder(int idx);
			void addFolder(FolderItem* folder);
			void removeFolder(FolderItem* folder);

			int numObjects() const;
			ObjectItem* object(int idx);
			void addObject(ObjectItem* object);
			void removeObject(ObjectItem* object);

			FolderItem* parent();
			PackageItem* owner();
			Folder* folder();

		protected:
			typedef std::list<FolderItem*> FolderItems;
			FolderItems mFolders;

			typedef std::list<ObjectItem*> ObjectItems;
			ObjectItems mObjects;

			FolderItem* mParent;
			PackageItem* mOwner;
			Folder* mFolder;
		};
	}
}

#endif // FOLDERITEM_INCLUDED
