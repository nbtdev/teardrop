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
class QWidget;
class QToolBox;
class QLabel;

namespace Teardrop
{
	namespace Tools
	{
		class QtPropertyGrid;
		class QtPackageExplorer;
		class Project;

		// temp
		class PackageManager;
		class Package;

		class Editor : public QMainWindow
		{
			Q_OBJECT

		public:
			Editor(QWidget *parent = 0, Qt::WFlags flags = 0);
			~Editor();

		protected slots:
			void onPackageExplorerItemClicked(QTreeWidgetItem*,int);
			void onContextMenu(const QPoint&);
			void onSave();
			void onSaveAs();
			void onOpen();
			void onNew();

			void onBeginLongOperation();
			void onEndLongOperation();

		private:
			Ui::EditorClass ui;
			QtPropertyGrid* mPropGrid;
			QtPackageExplorer* mPkgExp;
			PackageManager* mPkg;
			PackageManager* mPkgMgr;
			QWidget* m3DView;
			QToolBox* mClasses;
			QLabel* mPropGridDesc;

			Project* mProject;

			QCursor mCursor;

			void setEditorTitle();
		};
	}
}

#endif // EDITOR_H
