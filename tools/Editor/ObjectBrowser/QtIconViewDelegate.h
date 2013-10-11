/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(QTICONVIEWDELEGATE_INCLUDED)
#define QTICONVIEWDELEGATE_INCLUDED

#include <QStyledItemDelegate>

namespace Teardrop 
{
	namespace Tools 
	{
		class QtIconViewDelegate : public QStyledItemDelegate
		{
		public:
			QtIconViewDelegate(QObject* parent=0);
			~QtIconViewDelegate();

			QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
		};
	}
}

#endif // QTICONVIEWDELEGATE_INCLUDED
