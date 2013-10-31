/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Executable.h"
#include "Logic.h"

using namespace Teardrop;

TD_CLASS_IMPL(Executable);

Executable::Executable()
{

}

Executable::~Executable()
{

}

bool Executable::onPreLoad()
{
	if (getLogic())
		return getLogic()->onPreLoad(this);

	return true;
}

bool Executable::onPostLoad()
{
	if (getLogic())
		return getLogic()->onPostLoad(this);

	return true;
}

bool Executable::onPreUnload()
{
	if (getLogic())
		return getLogic()->onPreUnload(this);

	return true;
}

bool Executable::onPostUnload()
{
	if (getLogic())
		return getLogic()->onPostUnload(this);

	return true;
}

void Executable::tick()
{
	if (getLogic())
		getLogic()->update(this);
}