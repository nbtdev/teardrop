/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "QtIconViewDelegate.h"
#include "ProjectExplorer/QtProjectItem.h"

using namespace Teardrop;
using namespace Tools;

QtIconViewDelegate::QtIconViewDelegate(QObject* parent)
	: QStyledItemDelegate(parent)
{

}

QtIconViewDelegate::~QtIconViewDelegate()
{

}

QSize QtIconViewDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QtProjectItem* item = static_cast<QtProjectItem*>(index.internalPointer());
	if (item) {
		QRect r = option.fontMetrics.boundingRect(QString(item->toString()));
		return QSize(128 + 16, 128 + r.height() + 8);
	}

	return QSize();
}

