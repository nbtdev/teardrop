/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "CameraControllerManager.h"
#include "CCMState_FreeCam.h"
#include "CCMState_OrbitCam.h"
#include "CCMState_Transition_NewTarget.h"

#define DIRECTINPUT_VERSION 0x0800
#include <DInput.h>

using namespace Teardrop;
//---------------------------------------------------------------------------
CameraControllerManager::CameraControllerManager(GfxCamera* pCam)
{
	m_pState = 0;
	m_pCam = pCam;
	m_bAlt = false;
	m_pNextState = TD_NEW CCMState_FreeCam(pCam);
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
		pNext = TD_NEW CCMState_OrbitCam(m_pCam);
	else
		pNext = TD_NEW CCMState_FreeCam(m_pCam);

	m_pNextState = TD_NEW CCMState_Transition_NewTarget(
		m_pCam, newObj, pNext);
}
//---------------------------------------------------------------------------
void CameraControllerManager::changeController(Controller type)
{
	switch (type)
	{
	case CCM_ORBIT:
		m_pNextState = TD_NEW CCMState_OrbitCam(m_pCam);
		break;

	case CCM_FREECAM:
		m_pNextState = TD_NEW CCMState_FreeCam(m_pCam);
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
