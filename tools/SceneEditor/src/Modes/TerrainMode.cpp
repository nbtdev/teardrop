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
#include "Tools/Tool.h"
#include "TerrainMode.h"
#include "Game/include/InputEvent.h"

using namespace CoS;
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
