/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
