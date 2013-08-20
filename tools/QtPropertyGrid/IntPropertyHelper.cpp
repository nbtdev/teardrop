/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "IntPropertyHelper.h"
#include "Reflection/Reflection.h"
#include "Reflection/PropertyDef.h"
#include "ThirdParty/Qt4/QtTreePropertyBrowser/qtpropertymanager.h"

using namespace Teardrop;
using namespace Reflection;
using namespace Tools;

IntPropertyHelper::IntPropertyHelper(QtIntPropertyManager* propMgr, QtProperty* prop, Reflection::Object* obj, const Reflection::PropertyDef* propDef)
	: mPropMgr(propMgr)
	, PropertyHelper(obj, propDef, prop)
{
	connect(propMgr, SIGNAL(valueChanged(QtProperty*, int)), this, SLOT(onValueChanged(QtProperty*, int)));
}

IntPropertyHelper::~IntPropertyHelper()
{
}

void IntPropertyHelper::onValueChanged(QtProperty* prop, int val)
{
	if (prop == mMyProp) {
		int i = val;
		mPropDef->setData(mObject, &i);
	}
}