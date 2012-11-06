/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "CoreInterface.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
static CoreInterface s_if;
//---------------------------------------------------------------------------
CoreInterface::CoreInterface()
{
	memset(this, 0, sizeof(CoreInterface));
}
//---------------------------------------------------------------------------
CoreInterface& CoreInterface::get()
{
	return s_if;
}
