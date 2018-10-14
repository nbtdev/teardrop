/******************************************************************************
Copyright (c) 2018 Teardrop Games

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
******************************************************************************/

#include "OrbitCamController.h"
#include "ComponentHost.h"
#include "InputFilter.h"
#include "Core/InputEvent.h"
#include "Util/Timer.h"
#include "Gfx/Camera.h"
#include "Memory/Memory.h"
#include "Math/Transform.h"
#include "Math/Vector4.h"
#include "Math/MathUtil.h"

#include <queue>
#include <assert.h>

using namespace Teardrop;
TD_CLASS_IMPL(OrbitCamController);

/*
	Implementation of InputFilter that handles input specific to manipulating
	an orbit cam around a single object or point in space
*/
class Teardrop::OrbitCamInputFilter : public InputFilter
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

	TD_DECLARE_ALLOCATOR();

private:
	bool filterMouseEvent(const InputEvent::Mouse& mouse);

	std::queue<Action> m_actions;
	Action::Type m_currentActionType;

	bool m_emit; // only emit orbit inputs if the LMB is down
};

OrbitCamInputFilter::OrbitCamInputFilter()
{
	m_currentActionType = Action::NONE;
	m_emit = false;
}

OrbitCamInputFilter::~OrbitCamInputFilter()
{
}

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

OrbitCamInputFilter::Action::Type OrbitCamInputFilter::getCurrentActionType()
{
	return m_currentActionType;
}

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

OrbitCamController::OrbitCamController()
{
	mInputFilter = 0;
	mTargetPos = Vector4(0,0,0,0);
	reset();
}

OrbitCamController::~OrbitCamController()
{
}

void OrbitCamController::reset()
{
	mAzimuth =
	mElevation = 0;
	mDist = 3;
	mZoomFactor = 0.3f;
}

bool OrbitCamController::initialize()
{
	if (CameraController::initialize()) {
		reset();

		mInputFilter = TD_NEW OrbitCamInputFilter;
		return true;
	}

	return false;
}

bool OrbitCamController::destroy()
{
	delete mInputFilter;
	return CameraController::destroy();
}

void OrbitCamController::injectMouseMove(int absX, int absY, int relX, int relY)
{
	absX, absY, relX, relY;
}

void OrbitCamController::injectMouseWheel(int absZ, int relZ)
{
	absZ, relZ;
}

void OrbitCamController::setTargetPos(const Vector4& targetPos)
{
	mTargetPos = targetPos;
}

void OrbitCamController::evaluate(float deltaT)
{
	assert (mInputFilter);
	if (!mInputFilter)
		return;

	float translateStep = -10 * mZoomFactor * deltaT;
	float rotateFactor = MathUtil::HALF_PI * deltaT / 8.f;

	OrbitCamInputFilter::Action action;
	while (mInputFilter->getNextAction(action))
	{
		switch(action.type)
		{
		case OrbitCamInputFilter::Action::RESET:
			reset();
			break;

		case OrbitCamInputFilter::Action::ROTATE:
			mAzimuth += rotateFactor * action.extra[0];
			mElevation += rotateFactor * action.extra[1];
			break;

		case OrbitCamInputFilter::Action::ZOOM:
			mDist += translateStep * action.extra[2];
			break;
		}
	}
}

bool OrbitCamController::update(float deltaT)
{
	if (!mCamera)
		return false;

	// enable/disable input filter
	if (mInputFilter) {
		if (getEnabled())
			mInputFilter->enable();
		else
			mInputFilter->disable();
	}
	
	// evaluate pending actions
	evaluate(deltaT);

	// figure out camera position from polar coordinates
	Quaternion qa(mAzimuth, Vector4::UNIT_Y);
	Quaternion qe(mElevation, Vector4::UNIT_X);
	Quaternion q(qa * qe);

	mCamera->setOrientation(q);

	// go along this direction vector by m_dist units
	Vector4 v(q * (Vector4::UNIT_Z * mDist));

	mCamera->setPosition(mTargetPos - v);
	mCamera->setLookAt(mTargetPos);

	bool rtn = mCamera->update();
	setPosition(mCamera->getPosition());
	setOrientation(mCamera->getOrientation());
	return rtn;
}

InputFilter* OrbitCamController::getInputFilter() const
{
	return mInputFilter;
}

void OrbitCamController::setDistanceToTarget(float units)
{
	mDist = units;
}

void OrbitCamController::setZoomStepFactor(float factor)
{
	mZoomFactor = MathUtil::clamp(factor, 0, 1);
}

void OrbitCamController::reinit(const Vector4& pos)
{
	mDist = (pos - mTargetPos).getLength();

	// reverse-engineer the angles -- first compare the axis of the quat
	// to the unit-y axis
	Quaternion q(mCamera->getOrientation());

	float angle;
	Vector4 axis;
	q.toAngleAxis(angle, axis);

	mElevation = MathUtil::acos(dot(Vector4::UNIT_Y, axis));

	Quaternion tmp(mElevation, Vector4::UNIT_Y);
	invert(tmp);
	q = q * tmp;

	q.toAngleAxis(mAzimuth, axis);
}
