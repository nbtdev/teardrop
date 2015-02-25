/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PROJECTEXPLORER_INCLUDED)
#define PROJECTEXPLORER_INCLUDED

#include "Util/Event.h"
#include <QtWidgets/QTreeView>
#include <list>

namespace Teardrop
{
	namespace Tools
	{
		class ProjectModel;
		class Project;
		class DragDropData;
		class ProjectItem;
		class PackageManager;

		class ProjectExplorer : public QTreeView
		{
			Q_OBJECT

		public:
			ProjectExplorer(QWidget* parent=0);
			~ProjectExplorer();

			void setProject(Project* project);

            Event<ProjectItem*> SelectionChanged;

		signals:
			void activePackageChanged(PackageManager* pkgMgr);

		protected slots:
			void onContextMenu(const QPoint&);
			void currentChanged(const QModelIndex &current, const QModelIndex &previous);

		protected:
			void dragMoveEvent(QDragMoveEvent* event);

			ProjectModel* mModel;
		};
	}
}

#endif // PROJECTEXPLORER_INCLUDED
