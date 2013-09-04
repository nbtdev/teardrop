/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "PropertyHelper.h"
#include "Reflection/Reflection.h"

using namespace Teardrop;
using namespace Reflection;
using namespace Tools;

PropertyHelper::PropertyHelper(Reflection::Object* obj, const Reflection::PropertyDef* propDef, QtProperty* prop)
	: mObject(obj)
	, mPropDef(propDef)
	, mMyProp(prop)
{
	obj->PropertyChanged.bind(fastdelegate::MakeDelegate(this, &PropertyHelper::propValueChanged));
}

PropertyHelper::~PropertyHelper()
{
	mObject->PropertyChanged.unbind(fastdelegate::MakeDelegate(this, &PropertyHelper::propValueChanged));
}

void PropertyHelper::propValueChanged(const Reflection::PropertyDef* propDef)
{
	onValueChanged(propDef);
}

void PropertyHelper::onValueChanged(const Reflection::PropertyDef* propDef)
{
}

