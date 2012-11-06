/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "View.h"
#include "ViewManager.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
ViewManager::ViewManager(View* pDefaultView)
{
	m_pActiveView = pDefaultView;
	m_pActiveTool = 0;
}
//---------------------------------------------------------------------------
ViewManager::ViewManager()
{
	m_pActiveView = 0;
	m_pActiveTool = 0;
}
//---------------------------------------------------------------------------
ViewManager::~ViewManager()
{
}
//---------------------------------------------------------------------------
void ViewManager::update(float deltaT, Data* pData)
{
	if (m_pActiveView)
		m_pActiveView->update(deltaT, pData);
}
//---------------------------------------------------------------------------
bool ViewManager::handleInput(const InputEvent& evt)
{
	if (m_pActiveView)
		return m_pActiveView->handleInput(evt);

	return false;
}
//---------------------------------------------------------------------------
void ViewManager::setActiveTool(Tool* pTool)
{
	m_pActiveTool = pTool;

	if (m_pActiveView)
		m_pActiveView->setActiveTool(pTool);
}
