/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Environment.h"
#include <memory.h>

using namespace Teardrop;
//---------------------------------------------------------------------------
static Environment s_env;
//---------------------------------------------------------------------------
Environment::Environment()
{
	memset(this, 0, sizeof(Environment));
	isOffline = 1;
}
//---------------------------------------------------------------------------
Environment& Environment::get()
{
	return s_env;
}
