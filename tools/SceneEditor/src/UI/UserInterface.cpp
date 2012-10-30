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
#include "UserInterface.h"
#include "SceneEditorFrame.h"

using namespace CoS;
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
bool UserInterface::handleInputEvent(const CoS::InputEvent& evt)
{
	bool rtn = false;
	OnInputEvent(evt, rtn);
	return rtn;
}
