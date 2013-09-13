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

		class QtProjectExplorer : public QTreeView
		{
			Q_OBJECT

		public:
			QtProjectExplorer(QWidget* parent=0);
			~QtProjectExplorer();

			void setProject(Project* project);

		protected slots:
			void onContextMenu(const QPoint&);

		protected:
			void dragEnterEvent(QDragEnterEvent* event);
			void dragMoveEvent(QDragMoveEvent* event);
			void dragLeaveEvent(QDragLeaveEvent* event);

			QtProjectModel* mModel;

			// for triggering/managing drag/drop (which ought already to be part of the Qt API...)
			void mouseMoveEvent(QMouseEvent* event);
			virtual void onDragStart(DragDropData* ddd);
			QPoint mMouseDown;
			bool mIsDragging;
		};
	}
}

#endif // QTPROJECTEXPLORER_INCLUDED
