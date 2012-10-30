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

#if !defined(CCMSTATE_TRANSITION_NEWTARGET_INCLUDED)
#define CCMSTATE_TRANSITION_NEWTARGET_INCLUDED

#include "CCMState.h"

/*
	CCMState implementation that performs an animated
	transition to a new target, usually in response to 
	a request to "frame selected in view". The state
	morphs the camera position directly (slerping the
	rotation if needed), so no CoS::CameraController 
	implementation is necessary.
*/

class CCMState_Transition_NewTarget : public CCMState
{
	CoS::GfxCamera* m_pCam;
	CoS::ZoneObject* m_pTarget;

public:
	CCMState_Transition_NewTarget(
		CoS::GfxCamera* pCam, 
		CoS::ZoneObject* pTarget,
		CCMState* pNextState=0);
	~CCMState_Transition_NewTarget();

	void activate();
	void deactivate();
	CCMState* update(float deltaT);
	bool handleInput(const CoS::InputEvent& event);

	size_t getType() const;
	void rotate(int dx, int dy);
	void dolly(int val);
	void strafe(int val);
	void lift(int val);

	COS_DECLARE_ALLOCATOR();
};

#endif // CCMSTATE_TRANSITION_NEWTARGET_INCLUDED
