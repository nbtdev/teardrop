/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "UserInterface.h"
#include "SceneEditorFrame.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
UserInterface::UserInterface(SceneEditorFrame* pShell)
{
	m_pShell = pShell;

	if (m_pShell)
		m_pShell->FrameClosing.connect(
			this, &UserInterface::onShellClosing);
}
//---------------------------------------------------------------------------
UserInterface::~UserInterface()
{
	if (m_pShell)
		m_pShell->FrameClosing.disconnect(
			this, &UserInterface::onShellClosing);
}
//---------------------------------------------------------------------------
PropertyGridControl* UserInterface::getPropGrid()
{
	return m_pShell->getPropGrid();
}
//---------------------------------------------------------------------------
ObjectTreeView* UserInterface::getObjectTreeView()
{
	return m_pShell->getObjectTreeView();
}
//---------------------------------------------------------------------------
StatusBar* UserInterface::getStatusBar()
{
	return m_pShell->getStatusBar();
}
//---------------------------------------------------------------------------
Toolbar* UserInterface::getToolBar()
{
	return m_pShell->getToolBar();
}
//---------------------------------------------------------------------------
void UserInterface::onShellClosing()
{
	UserInterfaceClosing();
}
//---------------------------------------------------------------------------
void UserInterface::showContextMenu(ContextMenu* pMenu, int x, int y)
{
	m_pShell->showContextMenu(pMenu, x, y);
}
//---------------------------------------------------------------------------
void UserInterface::showTemplateList(bool bShow)
{
	m_pShell->showTemplateList(bShow);
}
//---------------------------------------------------------------------------
void UserInterface::showMaterialEditor(bool bShow)
{
	m_pShell->showMaterialEditor(bShow);
}
//---------------------------------------------------------------------------
Reflection::Object* UserInterface::getSelectedTemplate()
{
	return m_pShell->getSelectedTemplate();
}
//---------------------------------------------------------------------------
bool UserInterface::handleInputEvent(const Teardrop::InputEvent& evt)
{
	bool rtn = false;
	OnInputEvent(evt, rtn);
	return rtn;
}
