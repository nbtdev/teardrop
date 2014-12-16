/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Logic.h"

using namespace Teardrop;

TD_CLASS_IMPL(Logic);

Logic::Logic()
{

}

Logic::~Logic()
{

}

bool Logic::attached(Executable* /*exe*/)
{
	return true;
}

bool Logic::onPreLoad(Executable* /*exe*/)
{
	return true;
}

bool Logic::onPostLoad(Executable* /*exe*/)
{
	return true;
}

bool Logic::onPreUnload(Executable* /*exe*/)
{
	return true;
}

bool Logic::onPostUnload(Executable* /*exe*/)
{
	return true;
}

bool Logic::detached(Executable* /*exe*/)
{
	return true;
}

bool Logic::update(Executable* /*exe*/)
{
	return true;
}

void Logic::injectMouseMove(int /*absX*/, int /*absY*/, int /*relX*//* =0 */, int /*relY*//* =0 */)
{

}

void Logic::injectMouseWheel(int /*absZ*/, int /*relZ*//* =0 */)
{

}
