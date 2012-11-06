/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "FirstPersonCamController.h"
#include "ZoneObject.h"
#include "Gfx/GfxCamera.h"
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
