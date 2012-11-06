/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "SpotLight.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
TD_CLASS_IMPL(SpotLight);
//---------------------------------------------------------------------------
SpotLight::SpotLight()
{
	setType(GfxLight::LT_SPOT);
}
//---------------------------------------------------------------------------
SpotLight::~SpotLight()
{
}
