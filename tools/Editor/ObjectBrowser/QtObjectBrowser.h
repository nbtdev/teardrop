/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(QTOBJECTBROWSER_INCLUDED)
#define QTOBJECTBROWSER_INCLUDED

#include <QWidget>
#include <QObject>
#include "FastDelegate.h"

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
		class QtObjectBrowserModel;
		class QtListViewDelegate;
		class QtIconViewDelegate;
		class QtProjectItem;

		class QtObjectBrowser : public QWidget
		{
			Q_OBJECT
		public:
			QtObjectBrowser(QWidget* parent);
			~QtObjectBrowser();

			// clear the browser
			void clear();

			// this is temporary, we should not be showing objects in project tree
			void onItemSelected(QtProjectItem* current);

			fastdelegate::FastDelegate1<QtProjectItem*> ItemClicked;

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

			QtObjectBrowserModel* mModel;
			QtListViewDelegate* mListViewDelegate;
			QtIconViewDelegate* mIconViewDelegate;
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

#endif // QTOBJECTBROWSER_INCLUDED
