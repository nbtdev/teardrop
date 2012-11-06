/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Prop.h"
#include "Component_Render.h"
#include "Component_BasicPhysics.h"
#include "SceneUtil.h"
#include "Gfx/GfxMesh.h"
#include "Physics/Body.h"
#include "Util/StringUtil.h"
#include "Util/Environment.h"
#include "Util/Hash.h"
#include "Memory/Memory.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
TD_CLASS_IMPL(Prop);
//---------------------------------------------------------------------------
Prop::Prop()
{
	m_bDynamic = true;
}
//---------------------------------------------------------------------------
Prop::~Prop()
{
}
//---------------------------------------------------------------------------
bool Prop::initialize()
{
	return ZoneObject::initialize();
}
//---------------------------------------------------------------------------
bool Prop::destroy()
{
	setObjectId(0);
	return ZoneObject::destroy();
}
//---------------------------------------------------------------------------
bool Prop::update(float deltaT)
{
	RenderComponent* pRend;
	if (findComponents(
		RenderComponent::getClassDef(), (Component**)&pRend))
	{
		pRend->update(deltaT);
	}

	PhysicsComponent* pPhy;
	if (findComponents(
		BasicPhysicsComponent::getClassDef(), (Component**)&pPhy))
	{
		if (!getDynamic() && m_bNeedsTransformUpdate)
		{
			pPhy->setWorldTransform(getTransform());
		}

		pPhy->update(deltaT);
	}

	ZoneObject::update(deltaT);

	return true;
}
