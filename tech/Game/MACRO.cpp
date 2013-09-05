/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "MACRO.h"
#include "Command.h"
#include "Component_Render.h"
#include "Component_Animation.h"
#include "Component_CharacterPhysics.h"
#include "Component_Interpolator.h"
#include "Component_CameraInterface.h"
#include "Animation/AnimationBlender.h"
#include "Gfx/GfxMesh.h"
#include "Physics/Body.h"
#include "Stream/Stream.h"
#include "Math/MathUtil.h"
#include "Math/Matrix44.h"
#include "Util/Environment.h"
#include "Util/Hash.h"
#include "Memory/Memory.h"

// TEST TEST TEST
#include "BeamWeapon.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
TD_CLASS_IMPL(MACRO);
//---------------------------------------------------------------------------
MACRO::MACRO()
{
	m_spineBoneIndex = size_t(-1);
	m_pitch = 0;
}
//---------------------------------------------------------------------------
MACRO::~MACRO()
{
}
//---------------------------------------------------------------------------
bool MACRO::_initialize()
{
	// spawning doesn't set the scale part of the transform
	Transform xform(getTransform());
	xform.scale = Vector4::UNIT_SCALE;
	setTransform(xform);

	// copy these directly
	m_maxForwardLinearAccel = getMaxForwardLinearAccel();
	m_maxReverseLinearAccel = getMaxReverseLinearAccel();
	m_linearSelfBrake = getLinearSelfBrake();
	m_radialSelfBrake = getRadialSelfBrake();
	m_rotationalSelfBrake = getRotationalSelfBrake();
	m_linearSelfBraking = getLinearSelfBraking();
	m_radialSelfBraking = getRadialSelfBraking();
	m_rotationalSelfBraking = getRotationalSelfBraking();

	// convert all those wacky degrees and km/h to values that kinematic needs
	// rad/s = deg/s / 360 * 2PI
	m_maxRadialAccel = getMaxRadialAccel() / 360 * MathUtil::TWO_PI;
	m_maxRotationalAccel = getMaxRotationalAccel() / 360 * MathUtil::TWO_PI;
	m_maxRadialVelocity = getMaxRadialVelocity() / 360 * MathUtil::TWO_PI;
	m_maxRotationalVelocity = getMaxRotationalVelocity() / 360 * MathUtil::TWO_PI;

	// m/s = km/h * 1000 / 3600
	m_maxForwardSpeed = getMaxForwardSpeed() * 1000 / 3600;
	m_maxReverseSpeed = getMaxReverseSpeed() * 1000 / 3600;

	// rad = deg / 360 * 2PI
	m_maxRotationalDisplacement = 
		getMaxRotationalDisplacement() / 360 * MathUtil::TWO_PI;

	// todo: get the actual AABB from the data...
	m_aabb.extents = Vector4::UNIT_SCALE * 2.5;
	m_radius = m_aabb.getRadius();

	return true;
}
//---------------------------------------------------------------------------
void MACRO::pitch(float value)
{
	m_pitch = value;
}
//---------------------------------------------------------------------------
bool MACRO::_destroy()
{
	return true;
}
//---------------------------------------------------------------------------
bool MACRO::_update(float deltaT)
{
	// evaluate kinematic movement
	evaluate(deltaT);

	float animSpeed = m_linearVelocity / m_maxForwardSpeed;

	// update if necessary -- this should first feed physics components 

	// update components...
	const Matrix44* pBoneCache = 0;
	size_t sz = 0;

	AnimationComponent* pAnimComp = 0;
	AnimationBlender* pBlender = 0;
	if (findComponents(
		AnimationComponent::getClassDef(), (Component**)&pAnimComp))
	{
		// first get the spine bone index if not got yet
		if (size_t(-1) == m_spineBoneIndex)
		{
			m_spineBoneIndex = pAnimComp->getBoneIndexByName(String("Spine"));
		}

		if (size_t(-1) != m_spineBoneIndex)
		{
			// make any adjustments needed for torso twist
			Quaternion rot;
			rot.fromAngleAxis(m_rotationalDisplacement, Vector4::UNIT_Y);
			Quaternion pitch;
			float ang = m_pitch * getMaxPitchAngle() / 360.f * MathUtil::PI;
			pitch.fromAngleAxis(ang, Vector4::UNIT_X);
			rot = pitch * rot;
			pAnimComp->setAdditionalBoneRotationByIndex(m_spineBoneIndex, rot);

			// make the camera (at least the FFP one) follow the spine angles
			CameraInterfaceComponent* pCamComp;
			if (findComponents(CameraInterfaceComponent::getClassDef(), (Component**)&pCamComp))
			{
				pCamComp->setPitchBias(ang);
				pCamComp->setYawBias(m_rotationalDisplacement);
			}
		}

		pAnimComp->update(deltaT * animSpeed);
		pBoneCache = pAnimComp->getBoneCache();
		sz = pAnimComp->getNumBones();
		pBlender = pAnimComp->getBlender();
	}

	CharacterPhysicsComponent* pPhy = 0;
	bool hasSupport = false;
	if (findComponents(
		CharacterPhysicsComponent::getClassDef(), (Component**)&pPhy))
	{
		hasSupport = pPhy->isSupported();

		// update ragdoll, if there is one
		pPhy->driveRagdoll(deltaT, pBlender, getTransform());
	}

	// when all components have finished, we can update the transform of 
	// this thing (if needed -- for now, update it every time)
	float approxLinVel = MathUtil::abs(m_linearVelocity);
	float approxRadVel = MathUtil::abs(m_radialVelocity);
	float approxRotVel = MathUtil::abs(m_rotationalDisplacement);
	if (!hasSupport || 
		approxLinVel > MathUtil::EPSILON || 
		approxRadVel > MathUtil::EPSILON || 
		approxRotVel > MathUtil::EPSILON)
	{
		yaw(m_radialDisplacement);
		translate(m_positionalDisplacement);
	}
	else
	{
		return true;
	}

	// get the velocity vector adjustment from the interpolator (if any)
	// TODO: account for rotational velocity adjustments
	Transform xform(getTransform());
	Vector4 velocityAdjust(Vector4::ZERO);
	Quaternion rotAdjust(Quaternion::IDENTITY);
	float angVelocityAdjust = 0;

	InterpolatorComponent* pInterp;
	if (findComponents(
		InterpolatorComponent::getClassDef(), (Component**)&pInterp))
	{
		pInterp->update(deltaT);
		pInterp->getVelocityAdjustment(xform.trans, velocityAdjust);
		pInterp->getRotationAdjustment(xform.rot, rotAdjust);

#if defined(_DEBUG) // and verbose logging?
		//char buf[256]; 
		//sprintf_s(buf, 256, "Correction vector (%f,%f,%f)\n", 
		//	velocityAdjust.x, velocityAdjust.y, velocityAdjust.z);
		//OutputDebugString(buf);
#endif

		float angle;
		Vector4 axis;
		rotAdjust.toAngleAxis(angle, axis);
		if (MathUtil::abs(angle) > 0.05f)
			angVelocityAdjust = angle;
	}

	if (pPhy)
	{
		Vector4 pos(xform.trans);
		Quaternion rot(xform.rot);

		// account for rotational adjustments
		//xform.rot = xform.rot * rotAdjust;
		//xform.rot.normalize();

		pPhy->setLinearVelocity(
			(xform.rot * Vector4::UNIT_Z) * m_linearVelocity + velocityAdjust);

		pPhy->setAngularVelocity(
			Vector4(0, m_radialVelocity + angVelocityAdjust, 0, 0));
		if (pPhy->update(deltaT))
		{
			// get the official world transform when done
			pPhy->getWorldTransform(xform); // just updates the trans for now

			// only update the transform if it actually changed...updating the transform 
			// has some side effects now (finding affecting lights, etc) that 
			// can be expensive...
			if (xform.trans != pos || xform.rot != rot)
			{
				setTransform(xform);
			}
		}
	}

	RenderComponent* pRend = 0;
	if (findComponents(RenderComponent::getClassDef(), (Component**)&pRend))
	{
		// for animated meshes
		pRend->updateMatrixPalette(pBoneCache, sz);
		pRend->update(deltaT);
	}

	return true;
}
//---------------------------------------------------------------------------
void MACRO::_handleCommand(const Command& cmd)
{
	float extra;
	int iExtra;

	switch(cmd.getVerb())
	{
	case Command::MOVE:
		cmd.getExtra(extra, 0);
		throttle(extra);
		break;

	case Command::TURN:
		cmd.getExtra(extra, 0);
		steer(extra);
		break;

	case Command::TWIST:
		cmd.getExtra(extra, 0);
		rudder(extra);
		break;

	case Command::PITCH:
		cmd.getExtra(extra, 0);
		pitch(-extra);
		break;

	case Command::FIRE:
		cmd.getExtra(iExtra, 0);
		fireWeaponGroup(iExtra);
		break;
	}
}
