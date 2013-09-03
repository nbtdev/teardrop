/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "ObjectMetadata.h"

using namespace Teardrop;
using namespace Tools;

TD_CLASS_IMPL(ObjectMetadata);

ObjectMetadata::ObjectMetadata()
	: mObject(0)
{
}

ObjectMetadata::ObjectMetadata(Reflection::Object* obj)
	: mObject(obj)
{
	PropertyChanged.bind(this, &ObjectMetadata::onPropertyChanged);
}

ObjectMetadata::~ObjectMetadata()
{

}

void ObjectMetadata::onPropertyChanged(const Reflection::PropertyDef* prop)
{

}
