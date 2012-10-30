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

#include "FirstPersonCamController.h"
#include "ZoneObject.h"
#include "Gfx/include/GfxCamera.h"
#include "Math/include/MathUtil.h"
#include "Math/include/Transform.h"
#include "Util/include/Environment.h"
#include <assert.h>

using namespace CoS;
//---------------------------------------------------------------------------
COS_CLASS_IMPL(FirstPersonCamController);
//---------------------------------------------------------------------------
FirstPersonCamController::FirstPersonCamController()
{
	setOffset(Vector4(0, 1.25f, 0.5f, 0));
}
//---------------------------------------------------------------------------
FirstPersonCamController::~FirstPersonCamController()
{
}
//---------------------------------------------------------------------------
bool FirstPersonCamController::update(float /*deltaT*/)
{
	/*
		TODO: Ultimately we want to be at the "eye" position, but for now, 
		put us up the character's Y axis pointing forward
	*/
	if (m_pTarget)
	{
		Transform xform(m_pTarget->getTransformWS());
		m_pCamera->setPosition(xform.trans);

		Quaternion yawBias; yawBias.fromAngleAxis(getYawBias(), Vector4::UNIT_Y);
		Quaternion pitchBias; pitchBias.fromAngleAxis(getPitchBias(), Vector4::UNIT_X);
		Quaternion rot(xform.rot * yawBias * pitchBias);
		m_pCamera->setOrientation(rot);

		m_pCamera->translate(getOffset());
		m_pCamera->setLookAt(xform.trans + rot * Vector4(0, 0, 100, 0));
		bool rtn = m_pCamera->update();
		setPosition(m_pCamera->getPosition());
		setOrientation(m_pCamera->getOrientation());
		return rtn;
	}

	return true;
}
