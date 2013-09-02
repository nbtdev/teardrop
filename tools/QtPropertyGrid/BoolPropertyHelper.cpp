/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "BoolPropertyHelper.h"
#include "Reflection/Reflection.h"
#include "Reflection/PropertyDef.h"
#include "QtPropertyBrowser/qtpropertymanager.h"

using namespace Teardrop;
using namespace Reflection;
using namespace Tools;

BoolPropertyHelper::BoolPropertyHelper(QtBoolPropertyManager* propMgr, QtProperty* prop, Reflection::Object* obj, const Reflection::PropertyDef* propDef)
	: mPropMgr(propMgr)
	, PropertyHelper(obj, propDef, prop)
{
	connect(propMgr, SIGNAL(valueChanged(QtProperty*, bool)), this, SLOT(onValueChanged(QtProperty*, bool)));
}

BoolPropertyHelper::~BoolPropertyHelper()
{
}

void BoolPropertyHelper::onValueChanged(QtProperty* prop, bool val)
{
	if (prop == mMyProp) {
		bool b = val;
		mPropDef->setData(mObject, &b);
	}
}