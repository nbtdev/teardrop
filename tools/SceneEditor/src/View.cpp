/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "View.h"
#include "Tools/Tool.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
View::View()
{
	m_pActiveTool = 0;
}
//---------------------------------------------------------------------------
View::~View()
{
}
//---------------------------------------------------------------------------
void View::update(float /*deltaT*/, Data* /*pData*/)
{
}
//---------------------------------------------------------------------------
bool View::handleInput(const InputEvent& /*evt*/)
{
	return false;
}
//---------------------------------------------------------------------------
void View::setActiveTool(Tool* pTool)
{
	m_pActiveTool = pTool;

	if (pTool)
		pTool->setActiveView(this);
}
//---------------------------------------------------------------------------
void View::getViewSize(Vector2& size)
{
	size.x = -1;
	size.y = -1;
}
//---------------------------------------------------------------------------
const GfxCamera* View::getCamera()
{
	return 0;
}
//---------------------------------------------------------------------------
bool View::isInside(int /*x*/, int /*y*/)
{
	return false;
}
//---------------------------------------------------------------------------
void View::activate(MessageBus* /*pMsgBus*/)
{
}
//---------------------------------------------------------------------------
void View::deactivate(MessageBus* /*pMsgBus*/)
{
}
