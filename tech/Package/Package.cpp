/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Package.h"
#include <algorithm>

using namespace Teardrop;

Package::Package()
{
}

Package::~Package()
{
}

void Package::add(Reflection::Object* object)
{
	mObjects.push_back(object);
}

void Package::remove(Reflection::Object* object)
{
	Objects::iterator it = std::find(mObjects.begin(), mObjects.end(), object);
	mObjects.erase(it);
}

const Objects& Package::objects() const
{
	return mObjects;
}
