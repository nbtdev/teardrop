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

#if !defined(OBJECTBROWSER_INCLUDED)
#define OBJECTBROWSER_INCLUDED

#include "Util/Event.h"
#include <QtWidgets/QWidget>
#include <QtCore/QObject>

class QtListView;
class QtIconView;
class QPushButton;
class QSplitter;
class QModelIndex;
class QAbstractItemView;

namespace Teardrop 
{
	namespace Tools 
	{
		class ObjectBrowserModel;
		class ListViewDelegate;
		class IconViewDelegate;
		class ProjectItem;

		class ObjectBrowser : public QWidget
		{
			Q_OBJECT
		public:
			ObjectBrowser(QWidget* parent);
			~ObjectBrowser();

			// clear the browser
			void clear();

			// this is temporary, we should not be showing objects in project tree
			void onItemSelected(ProjectItem* current);

            Event<ProjectItem*> ItemClicked;
            Event<ProjectItem*> ItemDoubleClicked;

		protected:
			QtListView* mListView;
			QtIconView* mIconView;
			QPushButton* mCmdList;
			QPushButton* mCmdIcon;
			QPushButton* mCmdHSplit;
			QPushButton* mCmdVSplit;
			QSplitter* mSplitter;

			enum ViewType {
				VIEW_ICON,
				VIEW_VSPLIT,
				VIEW_HSPLIT,
				VIEW_LIST
			};

			ViewType mViewType;
			void setViewType(ViewType viewType);

			ObjectBrowserModel* mModel;
			ListViewDelegate* mListViewDelegate;
			IconViewDelegate* mIconViewDelegate;
			void onContextMenu(QAbstractItemView* view, const QPoint& pt);

		protected slots:
			void onCmdList();
			void onCmdIcon();
			void onCmdHSplit();
			void onCmdVSplit();
			void onItemClicked(const QModelIndex&);
			void onItemDoubleClicked(const QModelIndex&);
			void onListContextMenu(const QPoint& pt);
			void onIconContextMenu(const QPoint& pt);
		};
	}
}

#endif // OBJECTBROWSER_INCLUDED
