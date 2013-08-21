/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#ifndef EDITOR_H
#define EDITOR_H

#include <QtGui/QMainWindow>
#include "ui_Editor.h"

class QTreeWidgetItem;

namespace Teardrop
{
	namespace Tools
	{
		class QtPropertyGrid;
		class QtPackageExplorer;

		class Editor : public QMainWindow
		{
			Q_OBJECT

		public:
			Editor(QWidget *parent = 0, Qt::WFlags flags = 0);
			~Editor();

		protected slots:
			void onPackageExplorerItemClicked(QTreeWidgetItem*,int);

		private:
			Ui::EditorClass ui;
			QtPropertyGrid* mPropGrid;
			QtPackageExplorer* mPkgExp;
		};
	}
}

#endif // EDITOR_H
