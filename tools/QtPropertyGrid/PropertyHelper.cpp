/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "PropertyHelper.h"

using namespace Teardrop;
using namespace Reflection;
using namespace Tools;

PropertyHelper::PropertyHelper(Reflection::Object* obj, const Reflection::PropertyDef* propDef, QtProperty* prop)
	: mObject(obj)
	, mPropDef(propDef)
	, mMyProp(prop)
{
}

PropertyHelper::~PropertyHelper()
{
}
