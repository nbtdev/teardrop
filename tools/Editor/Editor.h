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
#include "ui_Preferences.h"
#include "Preferences.h"

class QWidget;
class QToolBox;
class QLabel;

namespace Teardrop
{
	namespace Tools
	{
		class QtPropertyGrid;
		class QtProjectExplorer;
		class QtProjectItem;
		class QtObjectBrowser;
		class Project;

		class Editor : public QMainWindow
		{
			Q_OBJECT

		public:
			Editor(QWidget *parent = 0, Qt::WFlags flags = 0);
			~Editor();

		protected slots:
			void onContextMenu(const QPoint&);
			void onSave();
			void onSaveAs();
			void onOpen();
			void onNew();
			void onPreferences();

			void onBeginLongOperation();
			void onEndLongOperation();

		private:
			Ui::EditorClass ui;
			Ui::PreferencesDialog mDlgPreferences;
			Preferences mPreferences;

			QtPropertyGrid* mPropGrid;
			QtProjectExplorer* mProjectExp;
			QtObjectBrowser* mObjBrowser;
			QWidget* m3DView;
			QToolBox* mClasses;
			QLabel* mPropGridDesc;

			Project* mProject;

			QCursor mCursor;

			void setEditorTitle();
			void openProject(const QString& pathname);

			void onProjectExplorerSelectionChanged(QtProjectItem* item);
		};
	}
}

#endif // EDITOR_H
