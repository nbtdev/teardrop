/******************************************************************************
Copyright (c) 2015 Teardrop Games

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

#include "ChaseCamController.h"
#include "ComponentHost.h"
#include "ZoneObject.h"
#include "Gfx/Camera.h"
#include "Math/MathUtil.h"
#include "Math/Transform.h"
#include "Util/Environment.h"
#include <assert.h>

using namespace Teardrop;
//---------------------------------------------------------------------------
TD_CLASS_IMPL(ChaseCamController);
//---------------------------------------------------------------------------
ChaseCamController::ChaseCamController()
{
	setOffset(Vector4(-2, 5, -15, 0));
}
//---------------------------------------------------------------------------
ChaseCamController::~ChaseCamController()
{
}
//---------------------------------------------------------------------------
bool ChaseCamController::update(float /*deltaT*/)
{
	/*
		For now, just put ourselves back some distance and a bit to the upper
		right of the thing to which we are attached
	*/
    if (mTarget)
	{
        Transform xform(mTarget->getTransformWS());
        mCamera->setPosition(xform.trans);

		Quaternion yawBias; yawBias.fromAngleAxis(getYawBias(), Vector4::UNIT_Y);
		Quaternion pitchBias; pitchBias.fromAngleAxis(getPitchBias(), Vector4::UNIT_X);
		Quaternion rot(xform.rot * pitchBias * yawBias);
        mCamera->setOrientation(rot);

        mCamera->translate(getOffset());
        mCamera->setLookAt(xform.trans + rot * Vector4(0, 0, 100, 0));
        bool rtn = mCamera->update();
        setPosition(mCamera->getPosition());
        setOrientation(mCamera->getOrientation());
		return rtn;
	}

	return true;
}
