/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "UIFlashResource.h"
#include "Resource/ResourceManager.h"
#include "Util/Environment.h"
#include "Util/FourCC.h"
#include <assert.h>

using namespace Teardrop;
using namespace UI;
//---------------------------------------------------------------------------
const FourCC& FlashResource::RESOURCE_TYPE = FourCC('S','W','F','R');
//---------------------------------------------------------------------------
FlashResource::FlashResource()
{
}
//---------------------------------------------------------------------------
FlashResource::~FlashResource()
{
}
//---------------------------------------------------------------------------
bool FlashResource::destroy()
{
	return true;
}
//---------------------------------------------------------------------------
bool FlashResource::release()
{
	return true;
}
