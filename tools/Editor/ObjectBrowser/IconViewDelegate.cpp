/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "IconViewDelegate.h"
#include "ProjectExplorer/ProjectItem.h"

using namespace Teardrop;
using namespace Tools;

IconViewDelegate::IconViewDelegate(QObject* parent)
	: QStyledItemDelegate(parent)
{

}

IconViewDelegate::~IconViewDelegate()
{

}

QSize IconViewDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	ProjectItem* item = static_cast<ProjectItem*>(index.internalPointer());
	if (item) {
		QRect r = option.fontMetrics.boundingRect(QString(item->toString()));
		return QSize(128 + 16, 128 + r.height() + 8);
	}

	return QSize();
}

