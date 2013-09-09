/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "QtPropertyGridDelegate.h"
#include "QtPropertyGridItem.h"
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QPainter>
#include <QColorDialog>
#include "Reflection/Reflection.h"
#include "Math/Vector4.h"

using namespace Teardrop;
using namespace Tools;

QtPropertyGridDelegate::QtPropertyGridDelegate(QObject *parent /* = 0 */)
	: QItemDelegate(parent)
{

}

QtPropertyGridDelegate::~QtPropertyGridDelegate()
{

}

static void toQColor(QColor& qc, const Vector4& color) 
{
	qc.setRedF(color.x);
	qc.setGreenF(color.y);
	qc.setBlueF(color.z);
	qc.setAlphaF(color.w);
}

static void fromQColor(Vector4& color, const QColor& qc) 
{
	color.x = qc.redF();
	color.y = qc.greenF();
	color.z = qc.blueF();
	color.w = qc.alphaF();
}

QWidget* QtPropertyGridDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QtPropertyGridItem* item = static_cast<QtPropertyGridItem*>(index.internalPointer());
	if (item) {
		const Reflection::PropertyDef* prop = item->property();
		Reflection::Object* obj = item->object();

		if (item->isBoolean())
			return new QCheckBox(parent);
		else if (item->isEnum()) {
			int eVal;
			prop->getData(obj, &eVal);

			// make a combo box and fill it with the enum values
			QComboBox* cb = new QComboBox(parent);
			Reflection::ClassDef* classDef = obj->getDerivedClassDef();
			const Reflection::EnumDef* enumDef = classDef->findEnum(prop->getTypeName(), true);
			const Reflection::EnumValue* enumVal = enumDef->values();
			
			int selectedIdx = -1;
			while (enumVal) {
				cb->addItem(enumVal->id(), QVariant(enumVal->value()));
				
				int idx = cb->count()-1;
				if (enumVal->value() == eVal)
					selectedIdx = idx;
				
				enumVal = enumVal->next();
			}

			cb->setCurrentIndex(selectedIdx);
			return cb;
		}

		if (prop) {
			if (obj && String("ColorEditor") == prop->getEditor()) {
				QColorDialog* dlg = new QColorDialog;
				dlg->setOption(QColorDialog::ShowAlphaChannel, true);
				return dlg;
			}
		}
	}

	QLineEdit* le = new QLineEdit(parent);
	return le;
}

void QtPropertyGridDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	QtPropertyGridItem* item = static_cast<QtPropertyGridItem*>(index.internalPointer());
	if (item) {
		Reflection::Object* obj = item->object();
		const Reflection::PropertyDef* prop = item->property();

		if (item->isBoolean()) {
			if (obj && prop) {
				bool b;
				prop->getData(obj, &b);
				QCheckBox* cb = static_cast<QCheckBox*>(editor);
				cb->setCheckState(b ? Qt::Checked : Qt::Unchecked);
			}
		}
		else if (item->isEnum()) {

		}
		else if (String("ColorEditor") == prop->getEditor()) {
			Vector4 color;
			QColor qc;
			prop->getData(obj, &color);
			toQColor(qc, color);

			QColorDialog* dlg = static_cast<QColorDialog*>(editor);
			dlg->setCurrentColor(qc);
		}
		else {
			QString& str = index.model()->data(index, Qt::EditRole).toString();
			QLineEdit* le = static_cast<QLineEdit*>(editor);
			le->setText(str);
		}
	}
}

void QtPropertyGridDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QtPropertyGridItem* item = static_cast<QtPropertyGridItem*>(index.internalPointer());
	if (item) {
		const Reflection::PropertyDef* prop = item->property();
		Reflection::Object* obj = item->object();

		if (item->isBoolean()) {
			QCheckBox* cb = static_cast<QCheckBox*>(editor);
			bool b = cb->checkState() == Qt::Checked;
			model->setData(index, b, Qt::EditRole);
		}
		else if (item->isEnum()) {
			QComboBox* cb = static_cast<QComboBox*>(editor);
			QVariant val = cb->itemData(cb->currentIndex());
			model->setData(index, val, Qt::EditRole);
		}
		else if (prop && obj && String("ColorEditor") == prop->getEditor()) {
			QColor qc;
			QColorDialog* dlg = static_cast<QColorDialog*>(editor);
			qc = dlg->currentColor();

			Vector4 color;
			fromQColor(color, qc);
			prop->setData(obj, &color);
		}
		else {
			QLineEdit* le = static_cast<QLineEdit*>(editor);
			QString& str = le->text();
			model->setData(index, str, Qt::EditRole);
		}
	}
}

void QtPropertyGridDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	editor->setGeometry(option.rect);
}

void QtPropertyGridDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	painter->save();

	// only do this for the value cell
	if (index.column() == 1) {
		QtPropertyGridItem* item = static_cast<QtPropertyGridItem*>(index.internalPointer());
		if (item) {
			const Reflection::PropertyDef* prop = item->property();
			Reflection::Object* obj = item->object();

			if (obj && prop) {
				if (String("ColorEditor") == prop->getEditor()) {
					// make a QColor of the property values
					Vector4 color;
					QColor qc;
					prop->getData(obj, &color);
					toQColor(qc, color);

					QBrush brush(qc);
					painter->fillRect(option.rect, brush);
				}
			}
		}
	}

	painter->setPen(Qt::lightGray);
	painter->drawRect(option.rect);

	painter->restore();

	QItemDelegate::paint(painter, option, index);
}