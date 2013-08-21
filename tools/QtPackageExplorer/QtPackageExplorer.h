/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(QTPACKAGEEXPLORER_INCLUDED)
#define QTPACKAGEEXPLORER_INCLUDED

#include <QTreeWidget>
#include <list>

namespace Teardrop
{
	class Package;

	namespace Tools
	{
		class PackageItem;

		class QtPackageExplorer : public QTreeWidget
		{
		public:
			QtPackageExplorer(QWidget* parent=0);
			~QtPackageExplorer();

			void addPackage(Package* package);
			void removePackage(Package* package);

		protected:
			typedef std::list<PackageItem*> PackageItems;
			PackageItems mRootItems;
		};
	}
}

#endif // QTPACKAGEEXPLORER_INCLUDED
