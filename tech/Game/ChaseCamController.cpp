/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
