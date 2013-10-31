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
	class Scene;

	namespace Gfx {
		class Renderer;
	}

	namespace Tools
	{
		class PropertyGrid;
		class ProjectExplorer;
		class ProjectItem;
		class ObjectBrowser;
		class Project;
		class RenderWindow;
		class PackageManager;

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
			void onClose();

			void onBeginLongOperation();
			void onEndLongOperation();

			void onActivePackageChanged(PackageManager* pkgMgr);

		private:
			Ui::EditorClass ui;
			Ui::PreferencesDialog mDlgPreferences;
			RenderWindow* mRenderWindow;
			Preferences mPreferences;

			PropertyGrid* mPropGrid;
			ProjectExplorer* mProjectExp;
			ObjectBrowser* mObjBrowser;
			QWidget* m3DView;
			QToolBox* mClasses;
			QLabel* mPropGridDesc;

			Project* mProject;
			Scene* mScene;
			Gfx::Renderer* mRenderer;

			QCursor mCursor;

			void setEditorTitle();
			void openProject(const QString& pathname);

			void onProjectExplorerSelectionChanged(ProjectItem* item);
		};
	}
}

#endif // EDITOR_H
