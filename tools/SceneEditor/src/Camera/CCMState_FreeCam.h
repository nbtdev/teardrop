/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(CCMSTATE_FREECAM_INCLUDED)
#define CCMSTATE_FREECAM_INCLUDED

#include "CCMState.h"

/*
	CCMState implementation wrapper for the freecam controller
	in the editor.
*/

class CCMState_FreeCam : public CCMState
{
	Teardrop::GfxCamera* m_pCam;
	int m_dx, m_dy, m_dolly, m_strafe, m_lift;
	bool m_bActive;
	bool m_bRotate;
	bool m_bAlt;
	bool m_bCtrl;
	bool m_bShift;

public:
	CCMState_FreeCam(Teardrop::GfxCamera* pCam, CCMState* pNextState=0);
	~CCMState_FreeCam();

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

#endif // CCMSTATE_FREECAM_INCLUDED
