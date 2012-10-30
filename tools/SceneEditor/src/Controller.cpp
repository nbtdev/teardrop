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
#include "Controller.h"
#include "CoreInterface.h"
#include "View.h"
#include "UI/UserInterface.h"
#include "UI/Controls/PropertyGrid.h"
#include "UI/Controls/ObjectTreeView.h"
#include "UI/Controls/ObjectTreeNode.h"
#include "UI/Controls/StatusBar.h"
#include "UI/Controls/Toolbar.h"
#include "ViewManager.h"
#include "Modes/ModeManager.h"
#include "Camera/CameraControllerManager.h"
#include "Game/include/Input.h"
#include "Game/include/InputEvent.h"

using namespace CoS;
//---------------------------------------------------------------------------
Controller::Controller(Input* pInput, View* pDefaultView, UserInterface* pUI)
{
	CoreInterface& iface = CoreInterface::get();
	iface.pUI = pUI;
	iface.pMsgBus = this;

	m_pUI = pUI;
	pUI->getObjectTreeView()->connect(this);
	pUI->getPropGrid()->connect(this);

	m_pModeMgr = COS_NEW ModeManager();
	m_pModeMgr->setActiveView(pDefaultView);
//	m_pModeMgr->changeMode(ModeManager::MODE_OBJECT); // default to object

	m_pViewMgr = COS_NEW ViewManager(pDefaultView);

	m_pInput = pInput;
	m_pActiveView = pDefaultView;
	m_pActiveView->activate(this);

	m_pViewMgr->ActiveViewChanged.connect(this, &Controller::onActiveViewChanged);
	m_pUI->UserInterfaceClosing.connect(
		this, &Controller::onUserInterfaceClosing);

	iface.pViewMgr = m_pViewMgr;
	iface.pInput = m_pInput;
	iface.pModeMgr = m_pModeMgr;
	iface.pCurrentView = m_pActiveView;
	iface.pController = this;

	m_bActive = true;
}
//---------------------------------------------------------------------------
Controller::~Controller()
{
	if (m_pUI)
	{
		m_pUI->UserInterfaceClosing.disconnect(
			this, &Controller::onUserInterfaceClosing);
	}

	delete m_pViewMgr;
	delete m_pModeMgr;
}
//---------------------------------------------------------------------------
void Controller::activate(bool bActive)
{
	m_bActive = bActive;
}
//---------------------------------------------------------------------------
void Controller::onUserInterfaceClosing()
{
	if (m_pUI)
	{
		m_pUI->getObjectTreeView()->disconnect();
		m_pUI->getPropGrid()->disconnect();
	}
}
//---------------------------------------------------------------------------
bool Controller::update(float deltaT, Data* pData)
{
	// gather input
	InputEvent event;
	while (m_pInput->getNextEvent(event))
	{
		if (m_bActive)
		{
			// try the view first
			if (!m_pViewMgr->handleInput(event))
			{
				// then try the mode
				if (!m_pModeMgr->handleInputEvent(event, pData))
				{
					// then handle it ourselves if applicable
				}
			}
		}
		else
			m_pUI->handleInputEvent(event);
	}

	m_pModeMgr->update(deltaT, pData);
	m_pViewMgr->setActiveTool(m_pModeMgr->getActiveTool());
	m_pViewMgr->update(deltaT, pData);

	return true;
}
//---------------------------------------------------------------------------
void Controller::onActiveViewChanged(View* pView)
{
	m_pActiveView = pView;
	m_pModeMgr->setActiveView(pView);
}