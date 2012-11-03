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

#include "stdafx.h"
#include "CCMState_Transition_NewTarget.h"
#include "Math/AABB.h"
#include "Util/Hash.h"

using namespace CoS;
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
