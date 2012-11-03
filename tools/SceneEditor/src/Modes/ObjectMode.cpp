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
#include "ObjectMode.h"
#include "CoreInterface.h"
#include "MessageBus.h"
#include "UI/UserInterface.h"
#include "UI/ContextMenu.h"
#include "UI/Controls/ToolBar.h"
#include "Tools/SelectObjectTool.h"
#include "Tools/MoveObjectTool.h"
#include "Tools/RotateObjectTool.h"
#include "Tools/PlaceObjectTool.h"
#include "Tools/TransformGizmo.h"
#include "Game/InputEvent.h"
#include "Util/_String.h"

using namespace CoS;
//---------------------------------------------------------------------------
static enum MenuIDs
{
	ID_EditEntityMaterial = 10001,
};
static MenuEntry s_testSubmenu[] = {
	{ "Entry2", 0, 1011, 0 },
	{ 0, 0, 11, 1 },
	{ "Entry3", 0, 1012, 0 },
	{ 0, 0, 0, 0 },
};
static MenuEntry s_testMenu[] = {
	{ "Edit Material...", 0, ID_EditEntityMaterial, 0 },
	//{ "Main Entry 1", 0, 1001, 0 },
	//{ "SubMenu", s_testSubmenu, 2, 0 },
	//{ 0, 0, 1, 1 },
	//{ "Main Entry 2", 0, 1003, 0 },
	{ 0, 0, 0, 0 },
};
static ContextMenu s_contextMenu = { s_testMenu };
//---------------------------------------------------------------------------
ObjectMode::ObjectMode()
{
	m_pGizmo = COS_NEW TransformGizmo;
	m_pGizmo->initialize();
	m_pGizmo->hide();

	Tool* pTool = COS_NEW SelectObjectTool();
	m_tools.push_back(pTool);

	pTool = COS_NEW MoveObjectTool(*m_pGizmo);
	m_tools.push_back(pTool);

	pTool = COS_NEW RotateObjectTool(*m_pGizmo);
	m_tools.push_back(pTool);

	pTool = COS_NEW PlaceObjectTool();
	m_tools.push_back(pTool);

	m_currentTool = 0;
	m_pSelectedObject = 0;
	m_toolsetId = size_t(-1);

	CoreInterface& iface = CoreInterface::get();
	iface.pMsgBus->ObjectSelectionChanged.connect(
		this, &ObjectMode::selectedObjectChanged);
	iface.pMsgBus->ContextMenuSelection.connect(
		this, &ObjectMode::handleContextMenu);
}
//---------------------------------------------------------------------------
ObjectMode::~ObjectMode()
{
	CoreInterface& iface = CoreInterface::get();
	iface.pMsgBus->ContextMenuSelection.disconnect(
		this, &ObjectMode::handleContextMenu);
	iface.pMsgBus->ObjectSelectionChanged.disconnect(
		this, &ObjectMode::selectedObjectChanged);

	for (size_t i=0; i<m_tools.size(); ++i)
	{
		delete m_tools[i];
	}

	m_pGizmo->destroy();
	delete m_pGizmo;
}
//---------------------------------------------------------------------------
bool ObjectMode::update(float deltaT, Data* pData)
{
	if (m_currentTool < m_tools.size() && m_tools[m_currentTool])
		return m_tools[m_currentTool]->update(deltaT, pData);

	return true;
}
//---------------------------------------------------------------------------
bool ObjectMode::handleInputEvent(const InputEvent& event, Data* pData)
{
	if (m_currentTool < m_tools.size() && m_tools[m_currentTool])
		return m_tools[m_currentTool]->handleInputEvent(event, pData);

	return false;
}
//---------------------------------------------------------------------------
Tool* ObjectMode::getActiveTool()
{
	if (m_currentTool < m_tools.size())
		return m_tools[m_currentTool];

	return 0;
}
//---------------------------------------------------------------------------
void ObjectMode::_activeViewChanged()
{
}
//---------------------------------------------------------------------------
const Tool** ObjectMode::getTools()
{
	if (m_tools.size())
		return (const Tool**)&m_tools[0];

	return 0;
}
//---------------------------------------------------------------------------
size_t ObjectMode::getNumTools()
{
	return m_tools.size();
}
//---------------------------------------------------------------------------
void ObjectMode::selectedObjectChanged(
	void* /*sender*/, Reflection::Object* pObj)
{
	m_pSelectedObject = pObj;
}
//---------------------------------------------------------------------------
void ObjectMode::setActiveTool(Tool* /*pTool*/, size_t toolIdx)
{
	if (toolIdx < m_tools.size() && toolIdx != m_currentTool)
	{
		if (m_currentTool < m_tools.size())
			m_tools[m_currentTool]->deactivate();

		m_currentTool = toolIdx;
		m_tools[m_currentTool]->activate();
		m_tools[m_currentTool]->setSelectedObject(m_pSelectedObject);
	}
}
//---------------------------------------------------------------------------
ContextMenu* ObjectMode::getContextMenuDefinition()
{
	if (!m_pSelectedObject)
		return 0;

	return &s_contextMenu;
}
//---------------------------------------------------------------------------
void ObjectMode::handleContextMenu(void* sender, size_t id)
{
	if (sender != &s_contextMenu)
		return;

	switch (id)
	{
	case ID_EditEntityMaterial:
		CoreInterface::get().pUI->showMaterialEditor(true);
		break;
	}
}
