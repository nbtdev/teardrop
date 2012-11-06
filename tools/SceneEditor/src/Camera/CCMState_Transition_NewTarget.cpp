/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "CCMState_Transition_NewTarget.h"
#include "Math/AABB.h"
#include "Util/Hash.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
static const size_t s_type = hashString("CCMState_Transition_NewTarget");
//---------------------------------------------------------------------------
CCMState_Transition_NewTarget::CCMState_Transition_NewTarget(
	GfxCamera* pCam, ZoneObject* pTarget, CCMState* pNextState)
	: CCMState(pNextState)
{
	m_pCam = pCam;
	m_pTarget = pTarget;
}
//---------------------------------------------------------------------------
CCMState_Transition_NewTarget::~CCMState_Transition_NewTarget()
{
}
//---------------------------------------------------------------------------
size_t CCMState_Transition_NewTarget::getType() const
{
	return s_type;
}
//---------------------------------------------------------------------------
CCMState* CCMState_Transition_NewTarget::update(float /*deltaT*/)
{
	if (m_pTarget)
	{
		// look at the center of the object, not its origin (which often 
		// is not the center)
		m_pCam->setLookAt(m_pTarget->getBoundingBox().center);
	}

	m_pCam->update();

	return m_pNextState;
}
//---------------------------------------------------------------------------
void CCMState_Transition_NewTarget::activate()
{
}
//---------------------------------------------------------------------------
void CCMState_Transition_NewTarget::deactivate()
{
}
//---------------------------------------------------------------------------
void CCMState_Transition_NewTarget::rotate(int /*dx*/, int /*dy*/)
{
}
//---------------------------------------------------------------------------
void CCMState_Transition_NewTarget::dolly(int /*val*/)
{
}
//---------------------------------------------------------------------------
void CCMState_Transition_NewTarget::strafe(int /*val*/)
{
}
//---------------------------------------------------------------------------
void CCMState_Transition_NewTarget::lift(int /*val*/)
{
}
//---------------------------------------------------------------------------
bool CCMState_Transition_NewTarget::handleInput(const InputEvent& /*event*/)
{
	return false;
}
