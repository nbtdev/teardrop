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
