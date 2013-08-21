/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(OBJECTITEM_INCLUDED)
#define OBJECTITEM_INCLUDED

#include <QTreeWidgetItem>

namespace Teardrop
{
	namespace Reflection
	{
		class Object;
	}

	namespace Tools
	{
		class FolderItem;

		class ObjectItem : public QTreeWidgetItem
		{
		public:
			ObjectItem(FolderItem* parent, Reflection::Object* object);
			~ObjectItem();

			FolderItem* parent();
			Reflection::Object* object();

		protected:
			FolderItem* mParent;
			Reflection::Object* mObject;
		};
	}
}

#endif // OBJECTITEM_INCLUDED
