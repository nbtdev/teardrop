/******************************************************************************
Copyright (c) 2018 Teardrop Games

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
******************************************************************************/

#include "ListViewDelegate.h"
#include "ProjectExplorer/ProjectItem.h"
#include <QtGui/QPainter>

using namespace Teardrop;
using namespace Tools;

ListViewDelegate::ListViewDelegate(QObject* parent)
	: QStyledItemDelegate(parent)
{

}

ListViewDelegate::~ListViewDelegate()
{

}

void ListViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyleOptionViewItemV4 opt = option;
	initStyleOption(&opt, index);

	painter->save();

	// just draw the text
	ProjectItem* item = static_cast<ProjectItem*>(index.internalPointer());
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

QSize ListViewDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	ProjectItem* item = static_cast<ProjectItem*>(index.internalPointer());
	if (item) {
		QRect r = option.fontMetrics.boundingRect(QString(item->toString()));
		return QSize(r.width(), r.height());
	}

	return QSize();
}

