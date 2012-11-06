/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(CCMSTATE_TRANSITION_NEWTARGET_INCLUDED)
#define CCMSTATE_TRANSITION_NEWTARGET_INCLUDED

#include "CCMState.h"

/*
	CCMState implementation that performs an animated
	transition to a new target, usually in response to 
	a request to "frame selected in view". The state
	morphs the camera position directly (slerping the
	rotation if needed), so no Teardrop::CameraController 
	implementation is necessary.
*/

class CCMState_Transition_NewTarget : public CCMState
{
	Teardrop::GfxCamera* m_pCam;
	Teardrop::ZoneObject* m_pTarget;

public:
	CCMState_Transition_NewTarget(
		Teardrop::GfxCamera* pCam, 
		Teardrop::ZoneObject* pTarget,
		CCMState* pNextState=0);
	~CCMState_Transition_NewTarget();

	void activate();
	void deactivate();
	CCMState* update(float deltaT);
	bool handleInput(const Teardrop::InputEvent& event);

	size_t getType() const;
	void rotate(int dx, int dy);
	void dolly(int val);
	void strafe(int val);
	void lift(int val);

	TD_DECLARE_ALLOCATOR();
};

#endif // CCMSTATE_TRANSITION_NEWTARGET_INCLUDED
