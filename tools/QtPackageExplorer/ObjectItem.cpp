/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "ObjectItem.h"
#include "FolderItem.h"
#include "PackageManager/PackageManager.h"
#include "PackageManager/PackageMetadata.h"
#include "PackageManager/Metadata.h"
#include "Reflection/Reflection.h"
#include "Reflection/PropertyDef.h"
#include "Reflection/ClassDef.h"
#include "Util/_String.h"
#include <assert.h>

using namespace Teardrop;
using namespace Tools;

ObjectItem::ObjectItem(FolderItem* parent, Reflection::Object* object, const String& id)
	: mParent(parent)
	, mObject(object)
	, mObjId(id)
	, mChangingName(false)
{
	mPkgMgr = parent->packageManager();
	assert(mPkgMgr);

	if (mPkgMgr) {
		mMetadata = mPkgMgr->metadata()->findObjectMetadata(object);
		assert(mMetadata);

		if (mMetadata) {
			// get a name
			setText(0, (const char*)mMetadata->getName());
			mMetadata->NameChanged.bind(this, &ObjectItem::onNameChanged);
		}
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

Metadata* ObjectItem::metadata()
{
	return mMetadata;
}

const String& ObjectItem::id()
{
	return mObjId;
}

PackageExplorerItem::Type ObjectItem::itemType()
{
	return TYPE_OBJECT;
}

void ObjectItem::onNameChanged(const char* newName)
{
	if (!mChangingName) {
		mChangingName = true;
		setText(0, newName);
		mChangingName = false;
	}
}

void ObjectItem::labelChanged(const String& newLabel)
{
	if (!mChangingName) {
		mChangingName = true;
		mMetadata->setName(newLabel);
		mChangingName = false;
	}
}
