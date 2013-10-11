/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "ObjectDragDropData.h"

using namespace Teardrop;
using namespace Tools;

ObjectDragDropData::ObjectDragDropData(Reflection::Object* obj, const String& path, Package* pkg)
	: mObject(obj)
	, mPath(path)
	, mPackage(pkg)
{
}

ObjectDragDropData::~ObjectDragDropData()
{
}

DragDropData::Type ObjectDragDropData::type()
{
	return DDD_OBJECT;
}

const String& ObjectDragDropData::path()
{
	return mPath;
}

Reflection::Object* ObjectDragDropData::object()
{
	return mObject;
}

Package* ObjectDragDropData::package()
{
	return mPackage;
}
