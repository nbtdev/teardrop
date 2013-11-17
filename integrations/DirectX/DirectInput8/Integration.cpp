/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "Integration.h"
#include "InputDI8.h"

namespace Teardrop {
namespace DirectInput {

Integration::Integration()
	: mInput(0)
{
	load();
}

Integration::~Integration()
{
	unload();
}

void Integration::load()
{
	// for now, just create an instance of the DirectInput singleton
	mInput = TD_NEW Input();
}

void Integration::unload()
{
	// for now, just destroy and delete the instance of the DirectInput singleton
	mInput->destroy();
	delete mInput;
	mInput = 0;
}

} // namespace DirectInput
} // namespace Teardrop
