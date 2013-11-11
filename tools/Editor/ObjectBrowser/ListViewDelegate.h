/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(LISTVIEWDELEGATE_INCLUDED)
#define LISTVIEWDELEGATE_INCLUDED

#include <QStyledItemDelegate>

namespace Teardrop 
{
	namespace Tools 
	{
		class ListViewDelegate : public QStyledItemDelegate
		{
		public:
			ListViewDelegate(QObject* parent=0);
			~ListViewDelegate();

			void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
			QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
		};
	}
}

#endif // LISTVIEWDELEGATE_INCLUDED
