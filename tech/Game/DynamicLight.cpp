/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "DynamicLight.h"
#include "Zone.h"
#include "Math/Transform.h"
#include "Reflection/PropertyDef.h"

using namespace Teardrop;
using namespace Reflection;
//---------------------------------------------------------------------------
TD_CLASS_IMPL(DynamicLight);
//---------------------------------------------------------------------------
DynamicLight::DynamicLight()
{
}
//---------------------------------------------------------------------------
DynamicLight::~DynamicLight()
{
}
//---------------------------------------------------------------------------
bool DynamicLight::initialize()
{
	if (!ZoneObject::initialize())
		return false;

	m_pZone->addShadowCastingLight(this);

	return true;
}
//---------------------------------------------------------------------------
bool DynamicLight::destroy()
{
	if (m_pZone)
		m_pZone->removeShadowCastingLight(this);

	return ZoneObject::destroy();
}
//---------------------------------------------------------------------------
const Vector4& DynamicLight::getPosition() const
{
	DynamicLight* pThis = const_cast<DynamicLight*>(this);
	return pThis->getTransform().trans;
}
//---------------------------------------------------------------------------
void DynamicLight::notifyPropertyChangedLocal(const PropertyDef* pPropDef)
{
	// let base class have first crack at it
	ZoneObject::notifyPropertyChangedLocal(pPropDef);

	if (String("LightColor") == pPropDef->getName())
	{
		// update the underlying GfxLight color
		Vector4 col(getLightColor());
		setColor(col.x, col.y, col.z);
	}
}
