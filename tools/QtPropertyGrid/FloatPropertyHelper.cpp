/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "FloatPropertyHelper.h"
#include "Reflection/Reflection.h"
#include "Reflection/PropertyDef.h"
#include "ThirdParty/Qt4/QtTreePropertyBrowser/qtpropertymanager.h"

using namespace Teardrop;
using namespace Reflection;
using namespace Tools;

FloatPropertyHelper::FloatPropertyHelper(QtDoublePropertyManager* propMgr, QtProperty* prop, Reflection::Object* obj, const Reflection::PropertyDef* propDef)
	: mPropMgr(propMgr)
	, PropertyHelper(obj, propDef, prop)
{
	connect(propMgr, SIGNAL(valueChanged(QtProperty*, double)), this, SLOT(onValueChanged(QtProperty*, double)));
}

FloatPropertyHelper::~FloatPropertyHelper()
{
}

void FloatPropertyHelper::onValueChanged(QtProperty* prop, double val)
{
	if (prop == mMyProp) {
		float f = (float)val;
		mPropDef->setData(mObject, &f);
	}
}