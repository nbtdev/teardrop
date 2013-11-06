/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "PropertyGridItem.h"
#include "Reflection/Reflection.h"

using namespace Teardrop;
using namespace Tools;

PropertyGridItem::PropertyGridItem(Reflection::Object* obj, const Reflection::PropertyDef* propDef, PropertyGridItem* parent/* =0 */)
	: mObject(obj)
	, mProp(propDef)
	, mParent(parent)
{
	if (propDef)
		mName = (const char*)propDef->getName();
}

PropertyGridItem::PropertyGridItem(const QString& name, PropertyGridItem* parent/* =0 */)
	: mObject(0)
	, mProp(0)
	, mParent(parent)
	, mName(name)
{
}

PropertyGridItem::~PropertyGridItem()
{
	qDeleteAll(mChildren);
}

bool PropertyGridItem::isGroup() const
{
	return (mObject==0 && mProp==0);
}

bool PropertyGridItem::isNested() const
{
	return (mObject!=0 && mProp!=0 && mProp->isNested());
}

bool PropertyGridItem::isBoolean() const 
{
	return mProp!=0 && mProp->isBoolean();
}

bool PropertyGridItem::isPointer() const 
{
	return mProp!=0 && mProp->isPointer();
}

bool PropertyGridItem::isReadOnly() const
{
	return mProp!=0 && strstr(mProp->getEditor(), "ReadOnly")!=0;
}

bool PropertyGridItem::isEnum() const
{
	return mProp!=0 && mProp->isEnum();
}

PropertyGridItem* PropertyGridItem::parent() const
{
	return mParent;
}

PropertyGridItem* PropertyGridItem::child(int row) const
{
	return mChildren.at(row);
}

int PropertyGridItem::numChildren() const
{
	return mChildren.size();
}

int PropertyGridItem::row() const
{
	if (mParent)
		return mParent->mChildren.indexOf(const_cast<PropertyGridItem*>(this));

	return 0;
}

Reflection::Object* PropertyGridItem::object() const
{
	return mObject;
}

const Reflection::PropertyDef* PropertyGridItem::property() const
{
	return mProp;
}

const QString& PropertyGridItem::name() const
{
	return mName;
}

QString PropertyGridItem::valueAsString() const
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

			const Reflection::ClassDef* classDef = mObject->getDerivedClassDef();
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

void PropertyGridItem::append(PropertyGridItem* child)
{
	mChildren.append(child);
}

QString PropertyGridItem::nameTooltip() const
{
	return name();
}

QString PropertyGridItem::valueTooltip() const
{
	return valueAsString();
}

void PropertyGridItem::setAltValue(const QString& altValue)
{
	mAltValue = altValue;
}