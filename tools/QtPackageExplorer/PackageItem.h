/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PACKAGEITEM_INCLUDED)
#define PACKAGEITEM_INCLUDED

#include <QTreeWidgetItem>

namespace Teardrop
{
	class Package;

	namespace Tools
	{
		class FolderItem;

		class PackageItem : public QTreeWidgetItem
		{
		public:
			PackageItem(Package* package);
			~PackageItem();

			Package* package();
			FolderItem* root();

		protected:
			FolderItem* mRoot;
			Package* mPackage;
		};
	}
}

#endif // PACKAGEITEM_INCLUDED
