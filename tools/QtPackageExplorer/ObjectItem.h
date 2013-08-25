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

		class ObjectItem : public PackageExplorerItem
		{
		public:
			ObjectItem(FolderItem* parent, Reflection::Object* object, const String& objId);
			~ObjectItem();

			// PackageExplorerItemType implementation
			PackageExplorerItem::Type itemType();

			FolderItem* parent();
			Reflection::Object* object();
			const String& id();

		protected:
			FolderItem* mParent;
			Reflection::Object* mObject;
			String mObjId;
		};
	}
}

#endif // OBJECTITEM_INCLUDED