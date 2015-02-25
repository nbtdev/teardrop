/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Input.h"

using namespace Teardrop;

template<> Input* Singleton<Input>::mInst = 0;

Input::Input()
{
	assert(!mInst);
	mInst = this;
}

Input::~Input()
{
}
