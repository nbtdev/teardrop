/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(CCMSTATE_ORBITCAM_INCLUDED)
#define CCMSTATE_ORBITCAM_INCLUDED

#include "CCMState.h"

/*
	CCMState implementation wrapper for the orbit cam controller
	in the editor.
*/

class CCMState_OrbitCam : public CCMState
{
	Teardrop::GfxCamera* m_pCam;
	float m_radius;
	int m_dolly;
	int m_rx;
	int m_ry;
	float m_azimuth;
	float m_elevation;
	bool m_ctrl;
	bool m_orbiting;

public:
	CCMState_OrbitCam(Teardrop::GfxCamera* pCam, CCMState* pNextState=0);
	~CCMState_OrbitCam();

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

#endif // CCMSTATE_ORBITCAM_INCLUDED
