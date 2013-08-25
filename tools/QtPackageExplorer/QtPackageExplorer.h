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
	namespace Tools
	{
		class PackageManager;

		class QtPackageExplorer : public QTreeWidget
		{
			Q_OBJECT

		public:
			QtPackageExplorer(QWidget* parent=0);
			~QtPackageExplorer();

			void addPackage(PackageManager* packageMgr, bool setEditing=false);
			void removePackage(PackageManager* packageMgr);

		protected slots:
			void onContextMenu(const QPoint&);

		protected:
			void dragEnterEvent(QDragEnterEvent* event);
			void dragMoveEvent(QDragMoveEvent* event);
			void dragLeaveEvent(QDragLeaveEvent* event);
			void dropEvent(QDropEvent* event);

			typedef std::list<PackageManager*> PackageManagers;
			PackageManagers mPackages;
		};
	}
}

#endif // QTPACKAGEEXPLORER_INCLUDED
