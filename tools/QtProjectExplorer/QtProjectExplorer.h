/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(QTPROJECTEXPLORER_INCLUDED)
#define QTPROJECTEXPLORER_INCLUDED

#include <QTreeView>
#include <list>
#include "FastDelegate.h"

namespace Teardrop
{
	namespace Tools
	{
		class QtProjectModel;
		class Project;
		class DragDropData;
		class QtProjectItem;

		class QtProjectExplorer : public QTreeView
		{
			Q_OBJECT

		public:
			QtProjectExplorer(QWidget* parent=0);
			~QtProjectExplorer();

			void setProject(Project* project);

			fastdelegate::FastDelegate1<QtProjectItem*> SelectionChanged;

		protected slots:
			void onContextMenu(const QPoint&);
			void currentChanged(const QModelIndex &current, const QModelIndex &previous);

		protected:
			void dragMoveEvent(QDragMoveEvent* event);

			QtProjectModel* mModel;
		};
	}
}

#endif // QTPROJECTEXPLORER_INCLUDED
