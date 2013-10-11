/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "QtListViewDelegate.h"
#include "ProjectExplorer/QtProjectItem.h"
#include <QPainter>

using namespace Teardrop;
using namespace Tools;

QtListViewDelegate::QtListViewDelegate(QObject* parent)
	: QStyledItemDelegate(parent)
{

}

QtListViewDelegate::~QtListViewDelegate()
{

}

void QtListViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyleOptionViewItemV4 opt = option;
	initStyleOption(&opt, index);

	painter->save();

	// just draw the text
	QtProjectItem* item = static_cast<QtProjectItem*>(index.internalPointer());
	if (item) {
		QSize sz = sizeHint(opt, index);
		QString val(item->toString());
		QRectF rf = painter->boundingRect(opt.rect, Qt::AlignLeft, val);

		if (opt.state & QStyle::State_Selected) {
			QBrush brush(Qt::lightGray);
			painter->fillRect(rf, brush);
		}

		painter->drawText(rf, val);
	}
	else
		QStyledItemDelegate::paint(painter, option, index);

	painter->restore();
}

QSize QtListViewDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QtProjectItem* item = static_cast<QtProjectItem*>(index.internalPointer());
	if (item) {
		QRect r = option.fontMetrics.boundingRect(QString(item->toString()));
		return QSize(r.width(), r.height());
	}

	return QSize();
}

