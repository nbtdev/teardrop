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
#include "CameraControllerManager.h"
#include "CCMState_FreeCam.h"
#include "CCMState_OrbitCam.h"
#include "CCMState_Transition_NewTarget.h"

#define DIRECTINPUT_VERSION 0x0800
#include <DInput.h>

using namespace CoS;
//---------------------------------------------------------------------------
CameraControllerManager::CameraControllerManager(GfxCamera* pCam)
{
	m_pState = 0;
	m_pCam = pCam;
	m_bAlt = false;
	m_pNextState = COS_NEW CCMState_FreeCam(pCam);
	m_currentState = 0;
}
//---------------------------------------------------------------------------
CameraControllerManager::~CameraControllerManager()
{
	delete m_pState;
	delete m_pNextState;
}
//---------------------------------------------------------------------------
void CameraControllerManager::changeTarget(const Vector4& /*newPos*/)
{
}
//---------------------------------------------------------------------------
void CameraControllerManager::changeTarget(ZoneObject* newObj)
{
	CCMState* pNext = 0;

	if (newObj)
		pNext = COS_NEW CCMState_OrbitCam(m_pCam);
	else
		pNext = COS_NEW CCMState_FreeCam(m_pCam);

	m_pNextState = COS_NEW CCMState_Transition_NewTarget(
		m_pCam, newObj, pNext);
}
//---------------------------------------------------------------------------
void CameraControllerManager::changeController(Controller type)
{
	switch (type)
	{
	case CCM_ORBIT:
		m_pNextState = COS_NEW CCMState_OrbitCam(m_pCam);
		break;

	case CCM_FREECAM:
		m_pNextState = COS_NEW CCMState_FreeCam(m_pCam);
		break;
	}

	// hacky...
	if (m_pNextState->getType() == m_currentState)
	{
		delete m_pNextState;
		m_pNextState = 0;
	}
}
//---------------------------------------------------------------------------
bool CameraControllerManager::update(float deltaT)
{
	if (m_pNextState)
	{
		if (m_pState)
		{
			m_pState->deactivate();
			delete m_pState;
		}

		m_pState = m_pNextState;
		m_currentState = m_pState->getType();
		m_pState->activate();
		m_pNextState = 0;
	}

	if (m_pState)
		m_pNextState = m_pState->update(deltaT);

	return true;
}
//---------------------------------------------------------------------------
bool CameraControllerManager::handleInputEvent(const InputEvent& event)
{
	if (m_pState)
		return m_pState->handleInput(event);

	return false;
}
