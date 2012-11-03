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
#include "CCMState_OrbitCam.h"
#include "Math/MathUtil.h"
#include "Util/Hash.h"

using namespace CoS;
//---------------------------------------------------------------------------
static const size_t s_type = hashString("CCMState_OrbitCam");
//---------------------------------------------------------------------------
CCMState_OrbitCam::CCMState_OrbitCam(GfxCamera* pCam, CCMState* pNextState)
: CCMState(pNextState)
{
	m_pCam = pCam;
	m_dolly = 0;
	m_rx = 0;
	m_ry = 0;
	m_azimuth = 0;
	m_elevation = 0;
	m_ctrl = false;
	m_orbiting = false;
}
//---------------------------------------------------------------------------
CCMState_OrbitCam::~CCMState_OrbitCam()
{
}
//---------------------------------------------------------------------------
size_t CCMState_OrbitCam::getType() const
{
	return s_type;
}
//---------------------------------------------------------------------------
CCMState* CCMState_OrbitCam::update(float deltaT)
{
	Vector4 target(m_pCam->getLookAt());
	Vector4 pos(m_pCam->getPosition());

	if (m_dolly)
	{
		m_radius = (pos - target).getLength();
		m_radius += float(m_dolly) * deltaT;
		m_dolly = 0;
	}

	// put the camera at the target and turn it 180
	m_pCam->setPosition(target);
	m_pCam->yaw(MathUtil::PI);
	m_pCam->getOrientation().toAzimuthElevation(m_azimuth, m_elevation);

	m_pCam->yaw(float(m_rx) / 180 * MathUtil::PI);
	m_pCam->pitch(float(m_ry) / 180 * MathUtil::PI);
	//m_azimuth += float(m_rx) / 180 * MathUtil::PI;
	//m_elevation += float(m_ry) / 180 * MathUtil::PI;

	//Quaternion tmp(Quaternion::IDENTITY);
	//Quaternion a, e;
	//a.fromAngleAxis(m_azimuth, Vector4::UNIT_Y);
	//e.fromAngleAxis(m_elevation, Vector4::UNIT_X);
	//tmp = tmp * a; // yaw
	//tmp = tmp * e; // pitch

	//m_pCam->setOrientation(tmp);

	m_rx = m_ry = 0;

	// move the cam back out to the end of this new direction and turn it 
	// around again
	m_pCam->translate(Vector4::UNIT_Z * m_radius);
	m_pCam->yaw(MathUtil::PI);

	m_pCam->update();

	return 0;
}
//---------------------------------------------------------------------------
void CCMState_OrbitCam::activate()
{
	Vector4 target(m_pCam->getLookAt());
	Vector4 pos(m_pCam->getPosition());
	m_radius = (pos - target).getLength();
}
//---------------------------------------------------------------------------
void CCMState_OrbitCam::deactivate()
{
}
//---------------------------------------------------------------------------
void CCMState_OrbitCam::rotate(int dx, int dy)
{
	m_rx += dx;
	m_ry -= dy;
}
//---------------------------------------------------------------------------
void CCMState_OrbitCam::dolly(int val)
{
	m_dolly = -val;
}
//---------------------------------------------------------------------------
void CCMState_OrbitCam::strafe(int /*val*/)
{
}
//---------------------------------------------------------------------------
void CCMState_OrbitCam::lift(int /*val*/)
{
}
//---------------------------------------------------------------------------
bool CCMState_OrbitCam::handleInput(const InputEvent& evt)
{
	bool rtn = false;

	if (evt.evtType.type == InputEvent::KEYBOARD)
	{
		if (evt.key.keyCode == DIK_LCONTROL || evt.key.keyCode == DIK_RCONTROL)
		{
			m_ctrl = evt.key.bKeyDown;
			rtn = true;
		}
	}

	if (evt.evtType.type == InputEvent::MOUSE)
	{
		if (evt.mouse.btnsDown || evt.mouse.btnsReleased)
		{
			if (evt.mouse.btnsDown == InputEvent::BUTTON_RIGHT && m_ctrl)
			{
				m_orbiting = true;
				rtn = true;
			}

			if (evt.mouse.btnsReleased == InputEvent::BUTTON_RIGHT)
			{
				m_orbiting = false;
			}
		}

		if (evt.mouse.relZ)
		{
			dolly(evt.mouse.relZ);
			rtn = true;
		}

		if (m_orbiting && m_ctrl && (evt.mouse.relX || evt.mouse.relY))
		{
			rotate(evt.mouse.relX, evt.mouse.relY);
			rtn = true;
		}
	}

	return (rtn | m_orbiting);
}
