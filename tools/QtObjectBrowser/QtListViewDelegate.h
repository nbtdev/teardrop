/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(QTLISTVIEWDELEGATE_INCLUDED)
#define QTLISTVIEWDELEGATE_INCLUDED

#include <QStyledItemDelegate>

namespace Teardrop 
{
	namespace Tools 
	{
		class QtListViewDelegate : public QStyledItemDelegate
		{
		public:
			QtListViewDelegate(QObject* parent=0);
			~QtListViewDelegate();

			void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
			QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
		};
	}
}

#endif // QTLISTVIEWDELEGATE_INCLUDED
