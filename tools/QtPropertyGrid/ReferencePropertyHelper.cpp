/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "ReferencePropertyHelper.h"
#include "Reflection/Reflection.h"
#include "Reflection/PropertyDef.h"
#include "QtPropertyBrowser/qtpropertymanager.h"

using namespace Teardrop;
using namespace Reflection;
using namespace Tools;

ReferencePropertyHelper::ReferencePropertyHelper(QtStringPropertyManager* propMgr, QtProperty* prop, Reflection::Object* obj, const Reflection::PropertyDef* propDef)
: mPropMgr(propMgr)
, PropertyHelper(obj, propDef, prop)
{
}

ReferencePropertyHelper::~ReferencePropertyHelper()
{
}

void ReferencePropertyHelper::onValueChanged(QtProperty* prop, const QString& val)
{
	// no implementation, user cannot set the reference value directly
}

void ReferencePropertyHelper::onValueChanged(const Reflection::PropertyDef* propDef)
{
	// user has done something that changed the reference in the object's properties
	if (propDef == mPropDef) {
		// get data from object in string form and set the Qt property value as such
		Reflection::Object* obj = 0;
		mPropDef->getData(mObject, &obj);

		String sVal;
		if (obj) {
			obj->getObjectId().toString(sVal);
		}

		mPropMgr->setValue(mMyProp, (const char*)sVal);
	}
}
