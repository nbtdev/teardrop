/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "Tools/Tool.h"
#include "TerrainMode.h"
#include "Game/InputEvent.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
TerrainMode::TerrainMode()
{
	m_pCurrentTool = 0;
}
//---------------------------------------------------------------------------
TerrainMode::~TerrainMode()
{
	delete m_pCurrentTool;
}
//---------------------------------------------------------------------------
bool TerrainMode::update(float deltaT, Data* pData)
{
	if (m_pCurrentTool)
		return m_pCurrentTool->update(deltaT, pData);

	return true;
}
//---------------------------------------------------------------------------
bool TerrainMode::handleInputEvent(const InputEvent& event, Data* pData)
{
	if (m_pCurrentTool)
		return m_pCurrentTool->handleInputEvent(event, pData);

	return false;
}
//---------------------------------------------------------------------------
Tool* TerrainMode::getActiveTool()
{
	return m_pCurrentTool;
}
//---------------------------------------------------------------------------
void TerrainMode::_activeViewChanged()
{
}
//---------------------------------------------------------------------------
const Tool** TerrainMode::getTools()
{
	return 0;
}
//---------------------------------------------------------------------------
size_t TerrainMode::getNumTools()
{
	return 0;
}
//---------------------------------------------------------------------------
void TerrainMode::setActiveTool(Tool* /*pTool*/, size_t /*toolIdx*/)
{
}
