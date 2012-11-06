/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "InputFilter.h"
#include "Game/InputEvent.h"
#define DIRECTINPUT_VERSION 0x0800
#include <DInput.h>

//---------------------------------------------------------------------------
InputFilter::InputFilter()
{
}
//---------------------------------------------------------------------------
InputFilter::~InputFilter()
{
}
//---------------------------------------------------------------------------
bool InputFilter::getNextAction(Action& action)
{
	if (!m_actions.size())
		return false;

	action = m_actions.front();
	m_actions.pop_front();
	return true;
}
//---------------------------------------------------------------------------
bool InputFilter::filterEvent(const Teardrop::InputEvent& event)
{
	Action action;
	if (event.evtType.type == Teardrop::InputEvent::KEYBOARD)
	{
		//switch (event.key.keyCode)
		//{
		//case DIK_GRAVE:
		//	if (!event.key.bKeyDown)
		//	{
		//		action.type = KEY;
		//		action.extra[0] = event.key.keyCode;
		//		m_actions.push_back(action);
		//		return true;
		//	}
		//}
	}
	else if (event.evtType.type == Teardrop::InputEvent::MOUSE)
	{
		action.type = MOUSE;
		action.extra[0] = event.mouse.relX;
		action.extra[1] = event.mouse.relY;
		action.extra[2] = event.mouse.relZ;
		action.extra[3] = event.mouse.absX;
		action.extra[4] = event.mouse.absY;
		action.extra[5] = event.mouse.absZ;
		action.extra[6] = event.mouse.btnsDown;
		action.extra[7] = event.mouse.btnsPressed;
		action.extra[8] = event.mouse.btnsReleased;
		action.isInWindow = event.mouse.isInside;
		m_actions.push_back(action);
		return true;
	}

	return false;
}
