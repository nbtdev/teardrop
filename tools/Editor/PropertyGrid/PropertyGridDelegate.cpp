/******************************************************************************
Copyright (c) 2015 Teardrop Games

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

#include "PropertyGridDelegate.h"
#include "PropertyGridItem.h"
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtGui/QPainter>
#include <QtWidgets/QColorDialog>
#include "Reflection/Reflection.h"
#include "Math/Vector4.h"

using namespace Teardrop;
using namespace Tools;

PropertyGridDelegate::PropertyGridDelegate(QObject *parent /* = 0 */)
	: QItemDelegate(parent)
{

}

PropertyGridDelegate::~PropertyGridDelegate()
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

QWidget* PropertyGridDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	PropertyGridItem* item = static_cast<PropertyGridItem*>(index.internalPointer());
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
			const Reflection::ClassDef* classDef = obj->getDerivedClassDef();
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

void PropertyGridDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	PropertyGridItem* item = static_cast<PropertyGridItem*>(index.internalPointer());
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
            QString str = index.model()->data(index, Qt::EditRole).toString();
			QLineEdit* le = static_cast<QLineEdit*>(editor);
			le->setText(str);
		}
	}
}

void PropertyGridDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	PropertyGridItem* item = static_cast<PropertyGridItem*>(index.internalPointer());
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
            QString str = le->text();
			model->setData(index, str, Qt::EditRole);
		}
	}
}

void PropertyGridDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	editor->setGeometry(option.rect);
}

void PropertyGridDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	painter->save();

	// only do this for the value cell
	if (index.column() == 1) {
		PropertyGridItem* item = static_cast<PropertyGridItem*>(index.internalPointer());
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
