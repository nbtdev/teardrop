/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "ChaseCamController.h"
#include "ComponentHost.h"
#include "Gfx/GfxCamera.h"
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
	if (m_pTarget)
	{
		Transform xform(m_pTarget->getTransformWS());
		m_pCamera->setPosition(xform.trans);

		Quaternion yawBias; yawBias.fromAngleAxis(getYawBias(), Vector4::UNIT_Y);
		Quaternion pitchBias; pitchBias.fromAngleAxis(getPitchBias(), Vector4::UNIT_X);
		Quaternion rot(xform.rot * pitchBias * yawBias);
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
