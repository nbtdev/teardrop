/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Affector.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
TD_CLASS_IMPL(Affector);
//---------------------------------------------------------------------------
Affector::Affector()
{
}
//---------------------------------------------------------------------------
Affector::~Affector()
{

}
//---------------------------------------------------------------------------
void Affector::onInstanceCreated()
{
	setServerComponent(false); // not needed on a server
}
//---------------------------------------------------------------------------
bool Affector::initialize()
{
	return Object::initialize();
}
//---------------------------------------------------------------------------
bool Affector::destroy()
{
	return Object::destroy();
}
//---------------------------------------------------------------------------
bool Affector::update(float deltaT)
{
	return true;
}
