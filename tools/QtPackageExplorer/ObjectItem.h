/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(OBJECTITEM_INCLUDED)
#define OBJECTITEM_INCLUDED

#include "QtPackageExplorer/PackageExplorerItem.h"
#include "Util/_String.h"

namespace Teardrop
{
	namespace Reflection
	{
		class Object;
	}

	namespace Tools
	{
		class FolderItem;
		class Metadata;
		class PackageManager;

		class ObjectItem : public PackageExplorerItem
		{
		public:
			ObjectItem(FolderItem* parent, Reflection::Object* object, const String& objId);
			~ObjectItem();

			// PackageExplorerItemType implementation
			PackageExplorerItem::Type itemType();

			FolderItem* parent();
			Reflection::Object* object();
			Metadata* metadata();
			const String& id();

			// Object callback - for when the underlying object's "Name" property was changed in code
			void onNameChanged(const char* newName);
			// PackageExplorerItem callback - for when the user changes the item's label in the explorer
			void labelChanged(const String& newLabel);

		protected:
			FolderItem* mParent;
			Reflection::Object* mObject;
			Metadata* mMetadata;
			PackageManager* mPkgMgr;
			String mObjId;
			bool mChangingName;
		};
	}
}

#endif // OBJECTITEM_INCLUDED
