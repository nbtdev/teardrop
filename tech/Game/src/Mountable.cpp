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

#include "Mountable.h"
#include "Game/include/Component_Render.h"
#include "Game/include/Component_EquipmentSlot.h"
#include "Math/include/Transform.h"
#include "Util/include/Environment.h"

using namespace CoS;
//---------------------------------------------------------------------------
COS_CLASS_IMPL(Mountable);
static Transform s_transform(Transform(
	Vector4(0,0,0,0), Quaternion(1,0,0,0), Vector4(1,1,1,1)));
//---------------------------------------------------------------------------
Mountable::Mountable()
{
	m_pSlot = 0;
}
//---------------------------------------------------------------------------
Mountable::~Mountable()
{
}
//---------------------------------------------------------------------------
bool Mountable::initialize()
{
	if (Object::initialize())
	{
		// initialize components
		for (Components::iterator it = m_components.begin();
			it != m_components.end(); ++it)
		{
			Component* pComp = it->second;
			if(!pComp->getServerComponent() && Environment::get().isServer)
				continue;

			pComp->initialize();
		}

		return _initialize();
	}

	return false;
}
//---------------------------------------------------------------------------
bool Mountable::destroy()
{
	_destroy();

	// clear out all components
	for (Components::iterator it = m_components.begin(); 
		it != m_components.end(); ++it)
	{
		it->second->destroy();
		delete it->second;
	}

	m_components.clear();

	return Object::destroy();
}
//---------------------------------------------------------------------------
bool Mountable::update(float deltaT)
{
	return _update(deltaT);
}
//---------------------------------------------------------------------------
const Transform& Mountable::getTransformWS()
{
	if (m_pSlot)
		return m_pSlot->getParentTransformWS();

	return s_transform; // static Transform is IDENTITY
}
//---------------------------------------------------------------------------
void Mountable::setBoundingBox(const AABB& /*aabb*/)
{
}
//---------------------------------------------------------------------------
bool Mountable::isOfType(Reflection::ClassDef* pClassDef)
{
	Reflection::ClassDef* pDerived = getDerivedClassDef();
	while (pDerived)
	{
		if (pDerived == pClassDef)
			return true;

		pDerived = pDerived->getBaseClass();
	}

	return false;
}
//---------------------------------------------------------------------------
void Mountable::notifyMounted(EquipmentSlotComponent* pSlot)
{
	m_pSlot = pSlot;
}
//---------------------------------------------------------------------------
void Mountable::notifyUnmounted()
{
	m_pSlot = 0;
}
//---------------------------------------------------------------------------
void Mountable::queueForRendering(GfxRenderer* pRend)
{
	ComponentList list;
	findComponents(RenderComponent::getClassDef(), list);

	for (ComponentList::iterator it = list.begin(); it != list.end(); ++it)
	{
		RenderComponent* pComp = static_cast<RenderComponent*>(*it);
		pComp->queueForRendering(pRend);
	}
}
//---------------------------------------------------------------------------
bool Mountable::isMounted() const
{
	return m_pSlot != 0;
}
//---------------------------------------------------------------------------
bool Mountable::_initialize()
{
	return true;
}
//---------------------------------------------------------------------------
bool Mountable::_destroy()
{
	return true;
}
//---------------------------------------------------------------------------
bool Mountable::_update(float /*deltaT*/)
{
	return true;
}
