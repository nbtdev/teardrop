/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Shape.h"
#include "Util/Environment.h"
#include "Util/Logger.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
Shape::Shape()
{
}
//---------------------------------------------------------------------------
Shape::~Shape()
{
}
//---------------------------------------------------------------------------
bool Shape::initialize()
{
	return true;
}
//---------------------------------------------------------------------------
bool Shape::release()
{
	return true;
}
//---------------------------------------------------------------------------
bool Shape::update(float /*deltaT*/)
{
	return true;
}
