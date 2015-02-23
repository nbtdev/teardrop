/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Metadata.h"
#include "Reflection/PropertyDef.h"
#include "Util/_String.h"

using namespace Teardrop;
using namespace Tools;

TD_CLASS_IMPL(Metadata);

Metadata::Metadata()
	: mObject(0)
{

}

Metadata::~Metadata()
{
}

void Metadata::notifyPropertyChangedLocal(const Reflection::PropertyDef* prop)
{
	if (String("Name") == prop->getName()) {
        NameChanged.raise((const char*)getName());
	}
}

void Metadata::generateThumbnail()
{
}

const Thumbnail& Metadata::thumbnail()
{
	return mThumb;
}

Reflection::Object* Metadata::object()
{
	return mObject;
}
