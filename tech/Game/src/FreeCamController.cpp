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

#include "FreeCamController.h"
#include "InputFilter.h"
#include "InputEvent.h"
#include "Util/include/Timer.h"
#include "Gfx/include/GfxCamera.h"
#include "Util/include/Environment.h"
#define DIRECTINPUT_VERSION 0x0800
#include <DInput.h>
#include "Memory/include/Memory.h"
#include "Math/include/MathUtil.h"

#include <queue>
#include <assert.h>

using namespace CoS;
//---------------------------------------------------------------------------
COS_CLASS_IMPL(FreeCamController);
//---------------------------------------------------------------------------
/*
	Implementation of InputFilter that handles input specific to moving a 
	free cam around the scene with mouse and keyboard
*/
class CoS::FreeCamInputFilter : public InputFilter
{
public:
	FreeCamInputFilter();
	~FreeCamInputFilter();

	struct Action
	{
		typedef enum
		{
			NONE,
			MOVE,
			STRAFE,
			ROTATE,
			ZOOM,
			RESET,
		} Type;

		Type type;
		float extra[3];
		bool bEnd;
	};

	//! InputFilter implementation
	bool filterEvent(const InputEvent& event);

	bool getNextAction(Action& /*in/out*/ action);
	Action::Type getCurrentActionType();

	COS_DECLARE_ALLOCATOR();

private:
	bool filterKeyEvent(const InputEvent::Key& key);
	bool filterMouseEvent(const InputEvent::Mouse& mouse);

	std::queue<Action> m_actions;
	Action::Type m_currentActionType;
};
//---------------------------------------------------------------------------
FreeCamInputFilter::FreeCamInputFilter()
{
	m_currentActionType = Action::NONE;
}
//---------------------------------------------------------------------------
FreeCamInputFilter::~FreeCamInputFilter()
{
}
//---------------------------------------------------------------------------
bool FreeCamInputFilter::getNextAction(Action& action)
{
	if (!m_actions.size())
	{
		return false;
	}

	action = m_actions.front();
	m_actions.pop();
	return true;
}
//---------------------------------------------------------------------------
bool FreeCamInputFilter::filterEvent(const InputEvent& event)
{
	switch(event.evtType.type)
	{
	case InputEvent::KEYBOARD:
		return filterKeyEvent(event.key);
		break;

	case InputEvent::MOUSE:
		return filterMouseEvent(event.mouse);
		break;
	}

	return false;
}
//---------------------------------------------------------------------------
bool FreeCamInputFilter::filterKeyEvent(const InputEvent::Key& key)
{
	Action action;
	action.type = Action::NONE;
	action.extra[0] = 
	action.extra[1] = 
	action.extra[2] = 0;
	action.bEnd = !key.bKeyDown;

	float factor = 1;
	if (!key.bKeyDown)
		factor = 0;

	bool bHandled = true;
	switch(key.keyCode)
	{
	case DIK_W:
	case DIK_UP:
		// move forward on local Z axis one unit
		action.type = Action::MOVE;
		action.extra[0] = 1 * factor;
		break;

	case DIK_A:
	case DIK_LEFT:
		// move left on local X axis one unit
		action.type = Action::STRAFE;
		action.extra[0] = 1 * factor;
		break;

	case DIK_S:
	case DIK_DOWN:
		// move backward on local Z axis one unit
		action.type = Action::MOVE;
		action.extra[0] = -1 * factor;
		break;

	case DIK_D:
	case DIK_RIGHT:
		// move right on local X axis one unit
		action.type = Action::STRAFE;
		action.extra[0] = -1 * factor;
		break;

	case DIK_ESCAPE:
	case DIK_SPACE:
		// move right on local X axis one unit
		action.type = Action::RESET;
		break;

	default:
		bHandled = false;
	}

	m_currentActionType = action.type;
	m_actions.push(action);
	return bHandled;
}
//---------------------------------------------------------------------------
FreeCamInputFilter::Action::Type FreeCamInputFilter::getCurrentActionType()
{
	return m_currentActionType;
}
//---------------------------------------------------------------------------
bool FreeCamInputFilter::filterMouseEvent(const InputEvent::Mouse& mouse)
{
	Action action;
	action.type = Action::NONE;
	action.extra[0] = action.extra[1] = action.extra[2] = 0;

	if (mouse.relX != 0 || mouse.relY != 0)
	{
		action.type = Action::ROTATE;
		action.extra[0] = -(float)mouse.relX;
		action.extra[1] = (float)mouse.relY;
	}

	if (mouse.relZ != 0)
	{
		action.type = Action::ZOOM;
		action.extra[2] = (float)mouse.relZ;
	}

	if (action.type != Action::NONE)
	{
		m_actions.push(action);
	}

	return false;
}
//---------------------------------------------------------------------------
FreeCamController::FreeCamController()
{
	m_pInputFilter = 0;
}
//---------------------------------------------------------------------------
FreeCamController::~FreeCamController()
{
}
//---------------------------------------------------------------------------
void FreeCamController::reset()
{
	setPosition(Vector4(100, 100, 100, 0));
	getOrientation().fromAngleAxis(0, Vector4::UNIT_Y);

	if (m_pCamera)
	{
		m_pCamera->setPosition(getPosition());
		//m_pCamera->setOrientation(m_orientation);
		m_pCamera->setLookAt(Vector4::ZERO);
	}

	m_move =
	m_strafe =
	m_zoom =
	m_rotX =
	m_rotY = 0;
}
//---------------------------------------------------------------------------
bool FreeCamController::initialize()
{
	if (CameraController::initialize())
	{
		reset();

		m_pInputFilter = COS_NEW FreeCamInputFilter;
		return true;
	}
	
	return false;
}
//---------------------------------------------------------------------------
bool FreeCamController::destroy()
{
	delete m_pInputFilter;
	return CameraController::destroy();
}
//---------------------------------------------------------------------------
void FreeCamController::evaluate(float deltaT)
{
	assert (m_pInputFilter);
	if (!m_pInputFilter)
	{
		return;
	}

	float translateStep = 100 * deltaT;
	float rotateFactor = MathUtil::HALF_PI * deltaT / 2.f;

	FreeCamInputFilter::Action action;
	while (m_pInputFilter->getNextAction(action))
	{
		switch(action.type)
		{
		case FreeCamInputFilter::Action::RESET:
			reset();
			break;

		case FreeCamInputFilter::Action::MOVE:
			m_move = action.extra[0] * translateStep;
			break;

		case FreeCamInputFilter::Action::STRAFE:
			m_strafe = action.extra[0] * translateStep;
			break;

		case FreeCamInputFilter::Action::ROTATE:
			m_rotX = rotateFactor * action.extra[0];
			m_rotY = rotateFactor * action.extra[1];
			break;

		case FreeCamInputFilter::Action::ZOOM:
			m_zoom = translateStep * action.extra[2] * 0.05f;
			break;
		}
	}
}
//---------------------------------------------------------------------------
bool FreeCamController::update(float /*deltaT*/)
{
	// enable/disable input filter
	if (m_pInputFilter)
	{
		if (getEnabled())
			m_pInputFilter->enable();
		else
			m_pInputFilter->disable();
	}

    Environment& env = Environment::get();
	// evaluate pending actions
	evaluate(env.pMasterClock->getElapsedTime());

	m_pCamera->yaw(m_rotX + getYawBias());
	m_pCamera->pitch(m_rotY = getPitchBias());
	m_pCamera->translate(Vector4(
		m_strafe, 
		0, 
		m_zoom + m_move, 
		0));

	m_rotX =
	m_rotY =
	m_zoom = 0; // one-shot application of mouse-driven stuff

	m_pCamera->setLookAt(
		m_pCamera->getPosition() + 
		m_pCamera->getOrientation() * Vector4(0, 0, 100, 0));

	bool rtn = m_pCamera->update();
	setPosition(m_pCamera->getPosition());
	setOrientation(m_pCamera->getOrientation());
	return rtn;
}
//---------------------------------------------------------------------------
InputFilter* FreeCamController::getInputFilter() const
{
	return m_pInputFilter;
}
