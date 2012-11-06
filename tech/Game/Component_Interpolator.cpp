/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Component_Interpolator.h"
#include "ZoneObject.h"
#include "Util/Environment.h"
#include "Util/Logger.h"
#include <assert.h>

using namespace Teardrop;
//---------------------------------------------------------------------------
TD_CLASS_IMPL(InterpolatorComponent);
//---------------------------------------------------------------------------
InterpolatorComponent::InterpolatorComponent()
{
	m_pHost = 0;
	m_syncPos = Vector4(0,0,0,0);
	m_syncInterval = 0;
	m_accumTime = 0;
	m_timeStep = 0;
}
//---------------------------------------------------------------------------
InterpolatorComponent::~InterpolatorComponent()
{
}
//---------------------------------------------------------------------------
void InterpolatorComponent::onInstanceCreated()
{
	setServerComponent(false); // not needed on a server
}
//---------------------------------------------------------------------------
bool InterpolatorComponent::update(float deltaT)
{
	m_accumTime += deltaT;
	m_timeStep = deltaT;
	return true;
}
//---------------------------------------------------------------------------
void InterpolatorComponent::setSync(
	const Vector4& pos, const Quaternion& rot)
{
	m_syncInterval = m_accumTime;
	m_accumTime = 0;
	m_syncPos = pos;
	m_syncRot = rot;

#if defined(_DEBUG)
	//{
	//	char buf[64];
	//	sprintf_s(buf, 64, "Target: (%f,%f,%f)\n",
	//		pos.x, pos.y, pos.z);
	//	Environment::get().pLogger->logMessage(buf);
	//}
#endif // _DEBUG
}
//---------------------------------------------------------------------------
void InterpolatorComponent::getRotationAdjustment(
	/*in*/ const Quaternion& currentRot,
	/*out*/ Quaternion& adjust)
{
	Quaternion q(currentRot);
	invert(q);
	adjust = q * m_syncRot;
}
//---------------------------------------------------------------------------
void InterpolatorComponent::getVelocityAdjustment(
	/*in*/ const Vector4& currentPos,
	/*out*/ Vector4& adjust)
{
	const float CLOSE_ENOUGH_SQR = 5e-4f;
	adjust = Vector4::ZERO;

	if (m_syncInterval == 0)
		return;

	Vector4 dir(m_syncPos - currentPos);

	// this may come back to bite us later -- but don't interpolate on
	// the Y axis because local gravity can deal with that
	dir.y = 0;

	float distSq = dir.getLength2();
	if (distSq < CLOSE_ENOUGH_SQR)
		return;

	// otherwise, figure out the velocity magnitude
	//float slice = m_timeStep;// / m_syncInterval;
	adjust = dir;// * slice;
}
