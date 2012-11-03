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

#include "OrbitCamController.h"
#include "ComponentHost.h"
#include "InputFilter.h"
#include "InputEvent.h"
#include "Util/Timer.h"
#include "Gfx/GfxCamera.h"
#include "Util/Environment.h"
#define DIRECTINPUT_VERSION 0x0800
#include <DInput.h>
#include "Memory/Memory.h"
#include "Math/Transform.h"
#include "Math/Vector4.h"
#include "Math/MathUtil.h"

#include <queue>
#include <assert.h>

using namespace CoS;
//---------------------------------------------------------------------------
COS_CLASS_IMPL(OrbitCamController);
//---------------------------------------------------------------------------
/*
	Implementation of InputFilter that handles input specific to moving a 
	free cam around the scene with mouse and keyboard
*/
class CoS::OrbitCamInputFilter : public InputFilter
{
public:
	OrbitCamInputFilter();
	~OrbitCamInputFilter();

	struct Action
	{
		typedef enum
		{
			NONE,
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
	bool filterMouseEvent(const InputEvent::Mouse& mouse);

	std::queue<Action> m_actions;
	Action::Type m_currentActionType;

	bool m_emit; // only emit orbit inputs if the LMB is down
};
//---------------------------------------------------------------------------
OrbitCamInputFilter::OrbitCamInputFilter()
{
	m_currentActionType = Action::NONE;
	m_emit = false;
}
//---------------------------------------------------------------------------
OrbitCamInputFilter::~OrbitCamInputFilter()
{
}
//---------------------------------------------------------------------------
bool OrbitCamInputFilter::getNextAction(Action& action)
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
bool OrbitCamInputFilter::filterEvent(const InputEvent& event)
{
	switch(event.evtType.type)
	{
	case InputEvent::MOUSE:
		return filterMouseEvent(event.mouse);
		break;
	}

	return false;
}
//---------------------------------------------------------------------------
OrbitCamInputFilter::Action::Type OrbitCamInputFilter::getCurrentActionType()
{
	return m_currentActionType;
}
//---------------------------------------------------------------------------
bool OrbitCamInputFilter::filterMouseEvent(const InputEvent::Mouse& mouse)
{
	if (mouse.btnsDown & 0x01)
	{
		//m_emit = mouse.isInside;
		m_emit = true;
	}
	if (mouse.btnsReleased & 0x01)
	{
		m_emit = false;
	}

	Action action;
	action.type = Action::NONE;
	action.extra[0] = action.extra[1] = action.extra[2] = 0;

	if (m_emit)
	{
		if (mouse.relX != 0 || mouse.relY != 0)
		{
			action.type = Action::ROTATE;
			action.extra[0] = (float)mouse.relX;
			action.extra[1] = (float)mouse.relY;
		}
	}

	//if (mouse.relZ != 0 && mouse.isInside)
	if (mouse.relZ)
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
OrbitCamController::OrbitCamController()
{
	m_pInputFilter = 0;
	m_targetPos = Vector4(0,0,0,0);
	m_dist = 3;
	m_zoomFactor = 0.3f;
}
//---------------------------------------------------------------------------
OrbitCamController::~OrbitCamController()
{
}
//---------------------------------------------------------------------------
void OrbitCamController::reset()
{
	m_azimuth =
	m_elevation = 0;
	m_dist = 3;
}
//---------------------------------------------------------------------------
bool OrbitCamController::initialize()
{
	if (CameraController::initialize())
	{
		reset();

		m_pInputFilter = COS_NEW OrbitCamInputFilter;
		return true;
	}

	return false;
}
//---------------------------------------------------------------------------
bool OrbitCamController::destroy()
{
	delete m_pInputFilter;
	return CameraController::destroy();
}
//---------------------------------------------------------------------------
void OrbitCamController::setTarget(ComponentHost* pTarget)
{
	m_pTarget = pTarget;
	if (m_pTarget)
	{
		m_targetPos = m_pTarget->getTransformWS().trans;
	}
}
//---------------------------------------------------------------------------
void OrbitCamController::evaluate(float deltaT)
{
	assert (m_pInputFilter);
	if (!m_pInputFilter)
	{
		return;
	}

	float translateStep = -10 * m_zoomFactor * deltaT;
	float rotateFactor = MathUtil::HALF_PI * deltaT / 8.f;

	OrbitCamInputFilter::Action action;
	while (m_pInputFilter->getNextAction(action))
	{
		switch(action.type)
		{
		case OrbitCamInputFilter::Action::RESET:
			reset();
			break;

		case OrbitCamInputFilter::Action::ROTATE:
			m_azimuth += rotateFactor * action.extra[0];
			m_elevation += rotateFactor * action.extra[1];
			break;

		case OrbitCamInputFilter::Action::ZOOM:
			m_dist += translateStep * action.extra[2];
			break;
		}
	}
}
//---------------------------------------------------------------------------
bool OrbitCamController::update(float /*deltaT*/)
{
	if (!m_pCamera)
		return false;

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

	// figure out camera position from polar coordinates
	Quaternion qa(m_azimuth, Vector4::UNIT_Y);
	Quaternion qe(m_elevation, Vector4::UNIT_X);
	Quaternion q(qa * qe);

	m_pCamera->setOrientation(q);

	// go along this direction vector by m_dist units
	Vector4 v(q * (Vector4::UNIT_Z * m_dist));

	Vector4 target(m_targetPos);
	if (m_pTarget)
	{
		target = m_pTarget->getTransformWS().trans;
	}

	m_pCamera->setPosition(target - v);
	m_pCamera->setLookAt(target);

	bool rtn = m_pCamera->update();
	setPosition(m_pCamera->getPosition());
	setOrientation(m_pCamera->getOrientation());
	return rtn;
}
//---------------------------------------------------------------------------
InputFilter* OrbitCamController::getInputFilter() const
{
	return m_pInputFilter;
}
//---------------------------------------------------------------------------
void OrbitCamController::setTarget(const Vector4& targetPos)
{
	m_targetPos = targetPos;
}
//---------------------------------------------------------------------------
void OrbitCamController::setDistanceToTarget(float units)
{
	m_dist = units;
}
//---------------------------------------------------------------------------
void OrbitCamController::setZoomStepFactor(float factor)
{
	m_zoomFactor = MathUtil::clamp(factor, 0, 1);
}
//---------------------------------------------------------------------------
void OrbitCamController::reinit(const Vector4& pos)
{
	m_dist = (pos - m_targetPos).getLength();

	// reverse-engineer the angles -- first compare the axis of the quat
	// to the unit-y axis
	Quaternion q(m_pCamera->getOrientation());

	float angle;
	Vector4 axis;
	q.toAngleAxis(angle, axis);

	m_elevation = MathUtil::acos(dot(Vector4::UNIT_Y, axis));

	Quaternion tmp(m_elevation, Vector4::UNIT_Y);
	invert(tmp);
	q = q * tmp;

	q.toAngleAxis(m_azimuth, axis);
}
