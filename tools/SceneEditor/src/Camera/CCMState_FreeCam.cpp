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

#include "stdafx.h"
#include "CCMState_FreeCam.h"
#include "Util/Hash.h"

using namespace CoS;
//---------------------------------------------------------------------------
static const float s_rotSpeed = 10.f; // max rad/s
static const float s_moveSpeed = 100.f; // constant move speed
static const size_t s_type = hashString("CCMState_FreeCam");
//---------------------------------------------------------------------------
CCMState_FreeCam::CCMState_FreeCam(GfxCamera* pCam, CCMState* pNextState)
: CCMState(pNextState)
{
	m_pCam = pCam;
	m_dx = m_dy = m_dolly = m_strafe = m_lift = 0;

	m_bAlt = m_bCtrl = m_bShift = m_bRotate = false;
	m_bActive = true;
}
//---------------------------------------------------------------------------
CCMState_FreeCam::~CCMState_FreeCam()
{
}
//---------------------------------------------------------------------------
size_t CCMState_FreeCam::getType() const
{
	return s_type;
}
//---------------------------------------------------------------------------
CCMState* CCMState_FreeCam::update(float deltaT)
{
	m_pCam->yaw((float)m_dx / 5.0f * s_rotSpeed * deltaT);
	m_pCam->pitch((float)m_dy / 5.0f * s_rotSpeed * deltaT);

	Vector4 lookAt(m_pCam->getOrientation() * (Vector4::UNIT_Z * 100));
	m_pCam->setLookAt(m_pCam->getPosition() + lookAt);

	if (m_strafe || m_dolly)
	{
		Vector4 v(Vector4::UNIT_Z * (float)m_dolly / 2.0f * s_moveSpeed * deltaT);

		m_pCam->translate(
			//Vector4(
			//	(float)m_strafe * s_moveSpeed * deltaT, 
			//	0, 
			//	(float)m_dolly / 2.0f * s_moveSpeed * deltaT, 
			//	0)
			v
			);
	}

	m_pCam->update();

	m_dx = m_dy = 0;
	if (MathUtil::abs(m_dolly) > 1)
		m_dolly = 0;

	return 0;
}
//---------------------------------------------------------------------------
void CCMState_FreeCam::activate()
{
}
//---------------------------------------------------------------------------
void CCMState_FreeCam::deactivate()
{
}
//---------------------------------------------------------------------------
void CCMState_FreeCam::rotate(int dx, int dy)
{
	m_dx = dx;
	m_dy = dy;
}
//---------------------------------------------------------------------------
void CCMState_FreeCam::dolly(int val)
{
	m_dolly = val;
}
//---------------------------------------------------------------------------
void CCMState_FreeCam::strafe(int val)
{
	m_strafe = val;
}
//---------------------------------------------------------------------------
void CCMState_FreeCam::lift(int /*val*/)
{
}
//---------------------------------------------------------------------------
bool CCMState_FreeCam::handleInput(const InputEvent& event)
{
	if (event.evtType.type == InputEvent::KEYBOARD)
	{
		switch(event.key.keyCode)
		{
		case DIK_LALT:
			m_bAlt = event.key.bKeyDown;
			break;

		case DIK_A:
			strafe(m_bRotate && event.key.bKeyDown ? -1 : 0);
			break;

		case DIK_D:
			strafe(m_bRotate && event.key.bKeyDown ? 1 : 0);
			break;

		case DIK_S:
			dolly(m_bRotate && event.key.bKeyDown ? -1 : 0);
			break;

		case DIK_W:
			dolly(m_bRotate && event.key.bKeyDown ? 1 : 0);
			break;
		}
	}

	if (event.evtType.type == InputEvent::MOUSE)
	{
		// we typically only act when left or right buttons are down
		const InputEvent::Mouse& evt = event.mouse;

		if (evt.btnsDown == InputEvent::BUTTON_RIGHT)
			m_bRotate = true;
		if (evt.btnsReleased == InputEvent::BUTTON_RIGHT)
		{
			m_bRotate = false;

			// also, cancel any current movements
			dolly(0);
			strafe(0);
			lift(0);
		}

		if (m_bRotate)
		{
			// rotate the camera
			rotate(event.mouse.relX, event.mouse.relY);
	
			return true;
		}
	}

	return false;
}
