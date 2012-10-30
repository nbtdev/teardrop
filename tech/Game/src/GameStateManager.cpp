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

#include "GameStateManager.h"
#include "GameState.h"

using namespace CoS;
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
