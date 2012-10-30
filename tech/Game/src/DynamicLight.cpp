/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#include "DynamicLight.h"
#include "Zone.h"
#include "Math/include/Transform.h"
#include "Reflection/include/PropertyDef.h"

using namespace CoS;
using namespace Reflection;
//---------------------------------------------------------------------------
COS_CLASS_IMPL(DynamicLight);
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
