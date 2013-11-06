/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Mountable.h"
#include "Game/Component_Render.h"
#include "Game/Component_EquipmentSlot.h"
#include "Math/Transform.h"
#include "Util/Environment.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
TD_CLASS_IMPL(Mountable);
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
Reflection::Object* Mountable::clone() const
{
	// leverage the base functionality...
	Reflection::Object* obj = Reflection::Object::clone();

	// and then do our components as well...
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
	const Reflection::ClassDef* pDerived = getDerivedClassDef();
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
