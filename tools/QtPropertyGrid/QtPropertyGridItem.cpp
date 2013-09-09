/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "QtPropertyGridItem.h"
#include "Reflection/Reflection.h"

using namespace Teardrop;
using namespace Tools;

QtPropertyGridItem::QtPropertyGridItem(Reflection::Object* obj, const Reflection::PropertyDef* propDef, QtPropertyGridItem* parent/* =0 */)
	: mObject(obj)
	, mProp(propDef)
	, mParent(parent)
{
	if (propDef)
		mName = (const char*)propDef->getName();
}

QtPropertyGridItem::QtPropertyGridItem(const QString& name, QtPropertyGridItem* parent/* =0 */)
	: mObject(0)
	, mProp(0)
	, mParent(parent)
	, mName(name)
{
}

QtPropertyGridItem::~QtPropertyGridItem()
{
	qDeleteAll(mChildren);
}

bool QtPropertyGridItem::isGroup() const
{
	return (mObject==0 && mProp==0);
}

bool QtPropertyGridItem::isBoolean() const 
{
	return mProp!=0 && mProp->isBoolean();
}

bool QtPropertyGridItem::isPointer() const 
{
	return mProp!=0 && mProp->isPointer();
}

bool QtPropertyGridItem::isReadOnly() const
{
	return mProp!=0 && strstr(mProp->getEditor(), "ReadOnly")!=0;
}

bool QtPropertyGridItem::isEnum() const
{
	return mProp!=0 && mProp->isEnum();
}

QtPropertyGridItem* QtPropertyGridItem::parent() const
{
	return mParent;
}

QtPropertyGridItem* QtPropertyGridItem::child(int row) const
{
	return mChildren.at(row);
}

int QtPropertyGridItem::numChildren() const
{
	return mChildren.size();
}

int QtPropertyGridItem::row() const
{
	if (mParent)
		return mParent->mChildren.indexOf(const_cast<QtPropertyGridItem*>(this));

	return 0;
}

Reflection::Object* QtPropertyGridItem::object() const
{
	return mObject;
}

const Reflection::PropertyDef* QtPropertyGridItem::property() const
{
	return mProp;
}

const QString& QtPropertyGridItem::name() const
{
	return mName;
}

QString QtPropertyGridItem::valueAsString() const
{
	String sVal;
	if (mObject) {
		if (mProp->isPointer() && mAltValue.length()) {
			// rather than the UUID, display the path
			sVal = mAltValue.toLatin1().data();
		}
		else if (isEnum()) {
			int eVal;
			mProp->getData(mObject, &eVal);

			Reflection::ClassDef* classDef = mObject->getDerivedClassDef();
			const Reflection::EnumDef* enumDef = classDef->findEnum(mProp->getTypeName(), true);
			if (enumDef) {
				const Reflection::EnumValue* enumVal = enumDef->findByValue(eVal);
				if (enumVal) {
					sVal = enumVal->id();
				}
			}
		}
		else {
			mProp->getDataAsString(mObject, sVal);
		}
	}

	return QString(sVal);
}

void QtPropertyGridItem::append(QtPropertyGridItem* child)
{
	mChildren.append(child);
}

QString QtPropertyGridItem::nameTooltip() const
{
	return name();
}

QString QtPropertyGridItem::valueTooltip() const
{
	return valueAsString();
}

void QtPropertyGridItem::setAltValue(const QString& altValue)
{
	mAltValue = altValue;
}