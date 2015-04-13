/******************************************************************************
Copyright (c) 2015 Teardrop Games

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
******************************************************************************/

#include "ZoneObject.h"
#include "Zone.h"
#include "Component.h"
#include "Stream/Stream.h"
#include "Memory/Memory.h"
#include "Util/Environment.h"
#include "Math/Transform.h"
#include "Math/Quaternion.h"
#include "Math/Vector4.h"
#include "Physics/Body.h"
#include <algorithm>

using namespace Teardrop;
using namespace Reflection;
//---------------------------------------------------------------------------
TD_CLASS_IMPL(ZoneObject);
//---------------------------------------------------------------------------
ZoneObject::ZoneObject() 
{
	m_radius = 0;
	m_bNeedsTransformUpdate = true;
	m_pZone = 0;
	m_aabbOffset = Vector4(0,0,0,0);
	m_bDynamic = false;
}
//---------------------------------------------------------------------------
ZoneObject::~ZoneObject()
{
}
//---------------------------------------------------------------------------
bool ZoneObject::shouldSerialize() const
{
	return true;
}
//---------------------------------------------------------------------------
bool ZoneObject::initialize()
{
	m_bNeedsTransformUpdate = true;

	// initialize all my components too
	for (Components::iterator it = m_components.begin();
		it != m_components.end(); ++it)
	{
		it->second->initialize();
	}

	return true;
}
//---------------------------------------------------------------------------
bool ZoneObject::destroy()
{
	// clear out all components
	for (Components::iterator it = m_components.begin(); 
		it != m_components.end(); ++it)
	{
		it->second->destroy();
		delete it->second;
	}

	m_components.clear();
	return true;
}
//---------------------------------------------------------------------------
Reflection::Object* ZoneObject::clone() const
{
	// Reflection::Object handles cloning the base properties
	Reflection::Object* obj = Reflection::Object::clone();
	ComponentHost* pHost = dynamic_cast<ComponentHost*>(obj);

	if (pHost) {
		// since we are also a component host, we need to clone our components
		// as well as the base properties
		int nComps = getComponents(0, 0);
		std::vector<Component*> compvec(nComps);
		Component** comps = &compvec[0];

		nComps = getComponents(comps, nComps);

		for (int i=0; i<nComps; ++i)
		{
			Component* pComp = comps[i];
			if(!pComp->getServerComponent() && Environment::get().isServer)
				continue;

			Component* pNewComp = static_cast<Component*>(pComp->clone());

			if (pNewComp)
			{
				pHost->addComponent(pNewComp);
			}
		}
	}

	return obj;
}
//---------------------------------------------------------------------------
void ZoneObject::setDynamic(bool isDynamic)
{
	m_bDynamic = isDynamic;
}
//---------------------------------------------------------------------------
void ZoneObject::setBoundingBox(const AABB& aabb)
{
	m_aabb = aabb;
	m_aabbOffset = m_aabb.center;
	m_radius = aabb.getRadius();
}
//---------------------------------------------------------------------------
const Transform& ZoneObject::getTransformWS()
{
	return getTransform();
}
//---------------------------------------------------------------------------
void ZoneObject::setRadius(float radius)
{
	m_radius = radius;
}
//---------------------------------------------------------------------------
void ZoneObject::notifyPropertyChangedLocal(
	const Reflection::PropertyDef* pPropDef)
{
	if (String("Transform") == pPropDef->getName())
	{
		// make sure the AABB center follows the transform trans component
		m_aabb.center = getTransform().trans + m_aabbOffset;

		// flag the update
		m_bNeedsTransformUpdate = true;
	}
}
//---------------------------------------------------------------------------
void ZoneObject::setPosition(const Vector4& pos)
{
	getTransform().trans = pos;
	// make sure the AABB center follows the transform trans component
	m_aabb.center = getTransform().trans + m_aabbOffset;
	m_bNeedsTransformUpdate = true;
}
//---------------------------------------------------------------------------
void ZoneObject::setOrientation(const Quaternion& q)
{
	//m_transform.rot = q;
	getTransform().rot = q;
	m_bNeedsTransformUpdate = true;
}
//---------------------------------------------------------------------------
void ZoneObject::yaw(float radians)
{
	Quaternion q;
	q.fromAngleAxis(radians, Vector4::UNIT_Y);
	getTransform().rot = getTransform().rot * q;
	m_bNeedsTransformUpdate = true;
}
//---------------------------------------------------------------------------
void ZoneObject::pitch(float radians)
{
	Quaternion q;
	q.fromAngleAxis(radians, Vector4::UNIT_X);
	getTransform().rot = getTransform().rot * q;
	m_bNeedsTransformUpdate = true;
}
//---------------------------------------------------------------------------
void ZoneObject::roll(float radians)
{
	Quaternion q;
	q.fromAngleAxis(radians, Vector4::UNIT_Z);
	getTransform().rot = getTransform().rot * q;
	m_bNeedsTransformUpdate = true;
}
//---------------------------------------------------------------------------
void ZoneObject::translate(const Vector4& trans)
{
	getTransform().trans += getTransform().rot * trans;
	// make sure the AABB center follows the transform trans component
	m_aabb.center = getTransform().trans + m_aabbOffset;
	m_bNeedsTransformUpdate = true;
}
//---------------------------------------------------------------------------
bool ZoneObject::update(float /*deltaT*/)
{
	if (m_pZone && m_bNeedsTransformUpdate)
	{
		// notify our containing zone -- we might need to be reclassified in 
		// spatial partitioning
		m_pZone->objectMoved(this);
	}

	return true;
}
//---------------------------------------------------------------------------
String ZoneObject::toString() const
{
	String tmp(getClassDef()->findProperty("Name")->getDefault());
	String trim = tmp.substr(1, tmp.length()-1);

	// FYI -- don't use emebedded objects like this directly, in case their
	// names change, you have to fix everywhere you did this...this is a 
	// special case due to the const-ness of the method
	if (___Name == trim)
		return String("Default Object");
	else
		return ___Name;
}
//---------------------------------------------------------------------------
void ZoneObject::setDistanceSqFrom(const Vector4& pt)
{
	m_distSq = (m_aabb.center - pt).getLength2();
}
//---------------------------------------------------------------------------
void ZoneObject::addedToZone(Zone* pZone)
{
	m_pZone = pZone;

	// alert components
	for (Components::iterator it = m_components.begin(); 
		it != m_components.end(); ++it)
	{
		it->second->addedToZone(pZone);
	}
}
//---------------------------------------------------------------------------
void ZoneObject::removeFromZone(Zone* pZone)
{
	// alert components
	for (Components::iterator it = m_components.begin(); 
		it != m_components.end(); ++it)
	{
		it->second->removeFromZone(pZone);
	}

	m_pZone = 0;
}
//---------------------------------------------------------------------------
void ZoneObject::setTemplateName(const String& name)
{
	m_templateName = name;
}
//---------------------------------------------------------------------------
bool ZoneObject::isOfType(Reflection::ClassDef* pClassDef)
{
	const Reflection::ClassDef* pDerived = getDerivedClassDef();
	while (pDerived)
	{
		if (pDerived == pClassDef)
			return true;

		pDerived = pDerived->getBaseClass();
	}

	return false;
}
