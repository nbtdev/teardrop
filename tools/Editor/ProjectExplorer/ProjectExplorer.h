/******************************************************************************
Copyright (c) 2018 Teardrop Games

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
******************************************************************************/

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
