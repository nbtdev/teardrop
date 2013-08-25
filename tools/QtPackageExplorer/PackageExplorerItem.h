/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PACKAGEEXPLORERITEM_INCLUDED)
#define PACKAGEEXPLORERITEM_INCLUDED

#include <QTreeWidgetItem>

namespace Teardrop
{
	namespace Tools
	{
		class PackageExplorerItem : public QTreeWidgetItem
		{
		public:
			enum Type
			{
				TYPE_FOLDER,
				TYPE_OBJECT,
			};

			virtual Type itemType() = 0;
		};
	}
}

#endif // PACKAGEEXPLORERITEM_INCLUDED
