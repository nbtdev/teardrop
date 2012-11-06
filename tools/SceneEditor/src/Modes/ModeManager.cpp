/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "CoreInterface.h"
#include "UI/UserInterface.h"
#include "UI/Controls/Toolbar.h"
#include "ModeManager.h"
#include "ObjectMode.h"
#include "TerrainMode.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
ModeManager::ModeManager()
{
	CoreInterface& iface = CoreInterface::get();
	m_currentMode = 0;
	m_pActiveView = 0;

	Toolbar* pToolbar = iface.pUI ? iface.pUI->getToolBar() : 0;

	Mode* pMode = TD_NEW ObjectMode();
	m_modes.push_back(pMode);
	if (pToolbar)
		pToolbar->addToolset("Object", pMode->getTools(), pMode->getNumTools());

	pMode = TD_NEW TerrainMode();
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
bool ModeManager::handleInputEvent(const Teardrop::InputEvent& event, Data* pData)
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
