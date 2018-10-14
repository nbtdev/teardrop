/******************************************************************************
Copyright (c) 2018 Teardrop Games

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
