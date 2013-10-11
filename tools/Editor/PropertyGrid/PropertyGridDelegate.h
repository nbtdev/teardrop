/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TEARDROP_PROPERTYGRIDDELEGATE_INCLUDED)
#define TEARDROP_PROPERTYGRIDDELEGATE_INCLUDED

#include <QItemDelegate>

namespace Teardrop
{
	namespace Tools
	{
		class PropertyGridDelegate : public QItemDelegate
		{
			Q_OBJECT

		public:
			explicit PropertyGridDelegate(QObject* parent = 0);
			~PropertyGridDelegate();

			// QItemDelegate implementation
			QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
			void setEditorData(QWidget* editor, const QModelIndex& index) const;
			void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
			void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;
			void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
		};
	}
}
#endif // TEARDROP_PROPERTYGRIDDELEGATE_INCLUDED
