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

class QListView;
class QPushButton;
class QSplitter;
class QModelIndex;

namespace Teardrop 
{
	namespace Tools 
	{
		class QtObjectBrowserModel;
		class QtListDelegate;
		class QtProjectItem;

		class QtObjectBrowser : public QWidget
		{
			Q_OBJECT
		public:
			QtObjectBrowser(QWidget* parent);
			~QtObjectBrowser();

			// this is temporary, we should not be showing objects in project tree
			void onItemSelected(QtProjectItem* current);

			fastdelegate::FastDelegate1<QtProjectItem*> ItemClicked;

		protected:
			QListView* mListView;
			QListView* mIconView;
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
			QtListDelegate* mListDelegate;

		protected slots:
			void onCmdList();
			void onCmdIcon();
			void onCmdHSplit();
			void onCmdVSplit();
			void onItemClicked(const QModelIndex&);
		};
	}
}

#endif // QTOBJECTBROWSER_INCLUDED
