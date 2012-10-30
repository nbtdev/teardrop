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
#include "CoreInterface.h"
#include "UI/UserInterface.h"
#include "UI/Controls/Toolbar.h"
#include "ModeManager.h"
#include "ObjectMode.h"
#include "TerrainMode.h"

using namespace CoS;
//---------------------------------------------------------------------------
ModeManager::ModeManager()
{
	CoreInterface& iface = CoreInterface::get();
	m_currentMode = 0;
	m_pActiveView = 0;

	Toolbar* pToolbar = iface.pUI ? iface.pUI->getToolBar() : 0;

	Mode* pMode = COS_NEW ObjectMode();
	m_modes.push_back(pMode);
	if (pToolbar)
		pToolbar->addToolset("Object", pMode->getTools(), pMode->getNumTools());

	pMode = COS_NEW TerrainMode();
	m_modes.push_back(pMode);
	if (pToolbar)
		pToolbar->addToolset("Terrain", pMode->getTools(), pMode->getNumTools());

	if (pToolbar)
	{
		pToolbar->setToolset(0);
		pToolbar->ToolsetChanged.connect(
			this, &ModeManager::onModeChanged);
		pToolbar->ToolChanged.connect(
			this, &ModeManager::onToolChanged);
	}

	m_currentMode = 0;
	m_modes[m_currentMode]->setActiveTool(0, 0);

}
//---------------------------------------------------------------------------
ModeManager::~ModeManager()
{
	for (size_t i=0; i<m_modes.size(); ++i)
	{
		delete m_modes[i];
	}
}
//---------------------------------------------------------------------------
void ModeManager::onModeChanged(size_t idx)
{
	m_modes[m_currentMode]->deactivate();
	m_currentMode = idx;
	m_modes[m_currentMode]->activate();
	m_modes[m_currentMode]->setActiveView(m_pActiveView);
}
//---------------------------------------------------------------------------
void ModeManager::onToolChanged(Tool* pTool, size_t idx)
{
	m_modes[m_currentMode]->setActiveTool(pTool, idx);
}
//---------------------------------------------------------------------------
bool ModeManager::update(float deltaT, Data* pData)
{
	if (m_modes[m_currentMode])
		return m_modes[m_currentMode]->update(deltaT, pData);

	return false;
}
//---------------------------------------------------------------------------
bool ModeManager::handleInputEvent(const CoS::InputEvent& event, Data* pData)
{
	if (m_modes[m_currentMode])
	{
		if (!m_modes[m_currentMode]->handleInputEvent(event, pData))
		{
			if (event.evtType.type == InputEvent::MOUSE)
			{
				if (event.mouse.btnsDown == InputEvent::BUTTON_RIGHT)
				{
					// throw up context menu -- this blocks until the user
					// clicks to dismiss the menu or select an item from it
					CoreInterface& iface = CoreInterface::get();
					if (iface.pUI)
					{
						Environment::get().pInput->suspend();
						iface.pUI->showContextMenu(
							m_modes[m_currentMode]->getContextMenuDefinition(),
							event.mouse.absX,
							event.mouse.absY);
						Environment::get().pInput->resume();
					}
				}
			}
		}
	}

	return false;
}
//---------------------------------------------------------------------------
Tool* ModeManager::getActiveTool()
{
	if (m_modes[m_currentMode])
		return m_modes[m_currentMode]->getActiveTool();

	return 0;
}
//---------------------------------------------------------------------------
void ModeManager::setActiveView(View* pView)
{
	m_pActiveView = pView;
}
