/******************************************************************************
Copyright (c) 2015 Teardrop Games

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

#ifndef __TEARDROP_EDITOR_H__
#define __TEARDROP_EDITOR_H__

#include <QtWidgets/QMainWindow>
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
            Editor(QWidget *parent = 0, Qt::WindowFlags flags = 0);
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
			void onObjectBrowserItemDoubleClicked(ProjectItem* item);
		};
	}
}

#endif // __TEARDROP_EDITOR_H__
