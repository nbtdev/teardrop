/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(ICONVIEWDELEGATE_INCLUDED)
#define ICONVIEWDELEGATE_INCLUDED

#include <QStyledItemDelegate>

namespace Teardrop 
{
	namespace Tools 
	{
		class IconViewDelegate : public QStyledItemDelegate
		{
		public:
			IconViewDelegate(QObject* parent=0);
			~IconViewDelegate();

			QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
		};
	}
}

#endif // ICONVIEWDELEGATE_INCLUDED
