/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "QtPropertyGridDelegate.h"
#include <QLineEdit>
#include <QPainter>

using namespace Teardrop;
using namespace Tools;

QtPropertyGridDelegate::QtPropertyGridDelegate(QObject *parent /* = 0 */)
	: QItemDelegate(parent)
{

}

QtPropertyGridDelegate::~QtPropertyGridDelegate()
{

}

QWidget* QtPropertyGridDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QLineEdit* le = new QLineEdit(parent);
	return le;
}

void QtPropertyGridDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	QString& str = index.model()->data(index, Qt::EditRole).toString();
	QLineEdit* le = static_cast<QLineEdit*>(editor);
	le->setText(str);
}

void QtPropertyGridDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QLineEdit* le = static_cast<QLineEdit*>(editor);
	QString& str = le->text();
	model->setData(index, str, Qt::EditRole);
}

void QtPropertyGridDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	editor->setGeometry(option.rect);
}

void QtPropertyGridDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	painter->save();
	painter->setPen(Qt::lightGray);
	painter->drawRect(option.rect);
	painter->restore();

	QItemDelegate::paint(painter, option, index);
}