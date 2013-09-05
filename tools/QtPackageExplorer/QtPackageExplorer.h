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
#include "FastDelegate.h"

namespace Teardrop
{
	namespace Tools
	{
		class PackageManager;
		class DragDropData;

		class QtPackageExplorer : public QTreeWidget
		{
			Q_OBJECT

		public:
			QtPackageExplorer(QWidget* parent=0);
			~QtPackageExplorer();

			void addPackage(PackageManager* packageMgr, bool setEditing=false);
			// hack - avoid callback loop
			void _addPackage(PackageManager* packageMgr, bool setEditing=false);
			void removePackage(PackageManager* packageMgr);
			void clearAllPackages();

			fastdelegate::FastDelegate1<PackageManager*> PackageAdded;
			fastdelegate::FastDelegate1<PackageManager*> PackageRemoved;

		signals:
			void beginLongOperation();
			void endLongOperation();

		protected slots:
			void onContextMenu(const QPoint&);
			void onItemChanged(QTreeWidgetItem *item, int column);

		protected:
			void dragEnterEvent(QDragEnterEvent* event);
			void dragMoveEvent(QDragMoveEvent* event);
			void dragLeaveEvent(QDragLeaveEvent* event);
			void dropEvent(QDropEvent* event);
			bool dropMimeData(QTreeWidgetItem * newParentPtr, int index, const QMimeData * data, Qt::DropAction action);

			typedef std::list<PackageManager*> PackageManagers;
			PackageManagers mPackages;

			// for triggering onDragStart (which ought already to be part of the Qt API...)
			void mouseMoveEvent(QMouseEvent* event);
			virtual void onDragStart(DragDropData* ddd);
			QPoint mMouseDown;
			bool mIsDragging;
		};
	}
}

#endif // QTPACKAGEEXPLORER_INCLUDED
