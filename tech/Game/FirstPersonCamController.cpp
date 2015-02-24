/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "FirstPersonCamController.h"
#include "ZoneObject.h"
#include "Gfx/Camera.h"
#include "Math/MathUtil.h"
#include "Math/Transform.h"
#include "Util/Environment.h"
#include <assert.h>

using namespace Teardrop;
//---------------------------------------------------------------------------
TD_CLASS_IMPL(FirstPersonCamController);
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
    if (mTarget)
	{
        Transform xform(mTarget->getTransformWS());
        mCamera->setPosition(xform.trans);

		Quaternion yawBias; yawBias.fromAngleAxis(getYawBias(), Vector4::UNIT_Y);
		Quaternion pitchBias; pitchBias.fromAngleAxis(getPitchBias(), Vector4::UNIT_X);
		Quaternion rot(xform.rot * yawBias * pitchBias);
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
