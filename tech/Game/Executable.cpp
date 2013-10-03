/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Executable.h"

using namespace Teardrop;

TD_CLASS_IMPL(Executable);

Executable::Executable()
{

}

Executable::~Executable()
{

}

bool Executable::initialize()
{
	return true;
}

bool Executable::destroy()
{
	return true;
}

bool Executable::update(float /*deltaT*/)
{
	return true;
}
