/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "StringPropertyHelper.h"
#include "Reflection/Reflection.h"
#include "Reflection/PropertyDef.h"
#include "ThirdParty/Qt4/QtTreePropertyBrowser/qtpropertymanager.h"

using namespace Teardrop;
using namespace Reflection;
using namespace Tools;

StringPropertyHelper::StringPropertyHelper(QtStringPropertyManager* propMgr, QtProperty* prop, Reflection::Object* obj, const Reflection::PropertyDef* propDef)
	: mPropMgr(propMgr)
	, PropertyHelper(obj, propDef, prop)
{
	connect(mPropMgr, SIGNAL(valueChanged(QtProperty*, const QString&)), this, SLOT(onValueChanged(QtProperty*, const QString&)));
}

StringPropertyHelper::~StringPropertyHelper()
{
}

void StringPropertyHelper::onValueChanged(QtProperty* prop, const QString& val)
{
	if (prop == mMyProp)
		mPropDef->setDataFromString(mObject, val.toLatin1().data());
}