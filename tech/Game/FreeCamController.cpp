/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "FreeCamController.h"
#include "InputFilter.h"
#include "Core/InputEvent.h"
#include "Util/Timer.h"
#include "Gfx/Camera.h"
#include "Memory/Memory.h"
#include "Math/MathUtil.h"

#include <queue>
#include <assert.h>

using namespace Teardrop;
TD_CLASS_IMPL(FreeCamController);

/*
	Implementation of InputFilter that handles input specific to moving a 
	free cam around the scene with mouse and keyboard
*/
class Teardrop::FreeCamInputFilter : public InputFilter
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

	TD_DECLARE_ALLOCATOR();

private:
	bool filterKeyEvent(const InputEvent::Key& key);
	bool filterMouseEvent(const InputEvent::Mouse& mouse);

	std::queue<Action> m_actions;
	Action::Type m_currentActionType;
};

FreeCamInputFilter::FreeCamInputFilter()
{
	m_currentActionType = Action::NONE;
}

FreeCamInputFilter::~FreeCamInputFilter()
{
}

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
	case InputEvent::KC_W:
	case InputEvent::KC_UP:
		// move forward on local Z axis one unit
		action.type = Action::MOVE;
		action.extra[0] = 1 * factor;
		break;

	case InputEvent::KC_A:
	case InputEvent::KC_LEFT:
		// move left on local X axis one unit
		action.type = Action::STRAFE;
		action.extra[0] = 1 * factor;
		break;

	case InputEvent::KC_S:
	case InputEvent::KC_DOWN:
		// move backward on local Z axis one unit
		action.type = Action::MOVE;
		action.extra[0] = -1 * factor;
		break;

	case InputEvent::KC_D:
	case InputEvent::KC_RIGHT:
		// move right on local X axis one unit
		action.type = Action::STRAFE;
		action.extra[0] = -1 * factor;
		break;

	case InputEvent::KC_ESCAPE:
	case InputEvent::KC_SPACE:
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

FreeCamInputFilter::Action::Type FreeCamInputFilter::getCurrentActionType()
{
	return m_currentActionType;
}

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

FreeCamController::FreeCamController()
{
	mInputFilter = 0;
	reset();
}

FreeCamController::~FreeCamController()
{
}

void FreeCamController::reset()
{
	setPosition(Vector4(100, 100, 100, 0));
	getOrientation().fromAngleAxis(0, Vector4::UNIT_Y);

	if (mCamera)
	{
		mCamera->setPosition(getPosition());
		//m_pCamera->setOrientation(m_orientation);
		mCamera->setLookAt(Vector4::ZERO);
	}

	mMove =
	mStrafe =
	mZoom =
	mRotX =
	mRotY = 0;
}

bool FreeCamController::initialize()
{
	if (CameraController::initialize())
	{
		reset();

		mInputFilter = TD_NEW FreeCamInputFilter;
		return true;
	}
	
	return false;
}

bool FreeCamController::destroy()
{
	delete mInputFilter;
	return CameraController::destroy();
}

void FreeCamController::evaluate(float deltaT)
{
	assert (mInputFilter);
	if (!mInputFilter)
		return;

	float translateStep = 100 * deltaT;
	float rotateFactor = MathUtil::HALF_PI * deltaT / 2.f;

	FreeCamInputFilter::Action action;
	while (mInputFilter->getNextAction(action)) {
		switch(action.type) {
		case FreeCamInputFilter::Action::RESET:
			reset();
			break;

		case FreeCamInputFilter::Action::MOVE:
			mMove = action.extra[0] * translateStep;
			break;

		case FreeCamInputFilter::Action::STRAFE:
			mStrafe = action.extra[0] * translateStep;
			break;

		case FreeCamInputFilter::Action::ROTATE:
			mRotX = rotateFactor * action.extra[0];
			mRotY = rotateFactor * action.extra[1];
			break;

		case FreeCamInputFilter::Action::ZOOM:
			mZoom = translateStep * action.extra[2] * 0.05f;
			break;
		}
	}
}

bool FreeCamController::update(float deltaT)
{
	// enable/disable input filter
	if (mInputFilter)
	{
		if (getEnabled())
			mInputFilter->enable();
		else
			mInputFilter->disable();
	}

	// evaluate pending actions
	evaluate(deltaT);

	mCamera->yaw(mRotX + getYawBias());
	mCamera->pitch(mRotY = getPitchBias());
	mCamera->translate(Vector4(
		mStrafe, 
		0, 
		mZoom + mMove, 
		0));

	mRotX =
	mRotY =
	mZoom = 0; // one-shot application of mouse-driven stuff

	mCamera->setLookAt(
		mCamera->getPosition() + 
		mCamera->getOrientation() * Vector4(0, 0, 100, 0));

	bool rtn = mCamera->update();
	setPosition(mCamera->getPosition());
	setOrientation(mCamera->getOrientation());
	return rtn;
}

InputFilter* FreeCamController::getInputFilter() const
{
	return mInputFilter;
}
