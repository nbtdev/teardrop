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

#if !defined(CCMSTATE_FREECAM_INCLUDED)
#define CCMSTATE_FREECAM_INCLUDED

#include "CCMState.h"

/*
	CCMState implementation wrapper for the freecam controller
	in the editor.
*/

class CCMState_FreeCam : public CCMState
{
	CoS::GfxCamera* m_pCam;
	int m_dx, m_dy, m_dolly, m_strafe, m_lift;
	bool m_bActive;
	bool m_bRotate;
	bool m_bAlt;
	bool m_bCtrl;
	bool m_bShift;

public:
	CCMState_FreeCam(CoS::GfxCamera* pCam, CCMState* pNextState=0);
	~CCMState_FreeCam();

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

#endif // CCMSTATE_FREECAM_INCLUDED
