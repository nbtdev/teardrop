/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "ObjectItem.h"
#include "Reflection/Reflection.h"
#include "Reflection/PropertyDef.h"
#include "Reflection/ClassDef.h"
#include "Util/_String.h"

using namespace Teardrop;
using namespace Tools;

ObjectItem::ObjectItem(FolderItem* parent, Reflection::Object* object, const String& id)
	: mParent(parent)
	, mObject(object)
	, mObjId(id)
{
	const Reflection::PropertyDef* prop = object->getDerivedClassDef()->findProperty("Name", true);
	if (prop) {
		String val;
		prop->getDataAsString(object, val);
		setText(0, (const char*)val);
	}
}

ObjectItem::~ObjectItem()
{

}

FolderItem* ObjectItem::parent()
{
	return mParent;
}

Reflection::Object* ObjectItem::object()
{
	return mObject;
}

const String& ObjectItem::id()
{
	return mObjId;
}

PackageExplorerItem::Type ObjectItem::itemType()
{
	return TYPE_OBJECT;
}