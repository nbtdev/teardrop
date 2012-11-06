/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "GameStateManager.h"
#include "GameState.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
GameStateManager::GameStateManager()
{
	m_pPendingState = 0;
	m_pCurrentState = 0;
}
//---------------------------------------------------------------------------
GameStateManager::~GameStateManager()
{
}
//---------------------------------------------------------------------------
bool GameStateManager::initialize()
{
	return true;
}
//---------------------------------------------------------------------------
bool GameStateManager::destroy()
{
	if (m_pPendingState)
	{
		m_pPendingState->destroy();
		delete m_pPendingState;
		m_pPendingState = 0;
	}

	if (m_pCurrentState)
	{
		m_pCurrentState->destroy();
		delete m_pCurrentState;
		m_pCurrentState = 0;
	}

	return true;
}
//---------------------------------------------------------------------------
bool GameStateManager::changeState(GameState* pNextState)
{
	if (m_pPendingState)
	{
		return false;
	}

	m_pPendingState = pNextState;
	return true;
}
//---------------------------------------------------------------------------
bool GameStateManager::update()
{
	if (m_pPendingState)
	{
		if (m_pCurrentState)
		{
			m_pCurrentState->destroy();
			delete m_pCurrentState;
		}

		m_pCurrentState = m_pPendingState;
		m_pPendingState = 0;

		if (!m_pCurrentState->initialize())
		{
			return false;
		}
	}

	// if we don't have a state, exit with true so as not
	// to kill the loop
	if (!m_pCurrentState)
		return true;

	return m_pCurrentState->update();
}
