/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "CountdownTrigger.h"
#include "Math/MathUtil.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
CountdownTrigger::CountdownTrigger(float secondsUntilFire, bool repeat)
: m_repeat(repeat)
, m_secondsUntilFire(secondsUntilFire)
{
	m_countdownTimer = secondsUntilFire;
}
//---------------------------------------------------------------------------
CountdownTrigger::~CountdownTrigger()
{
}
//---------------------------------------------------------------------------
void CountdownTrigger::reset(float secondsUntilFire)
{
	if (secondsUntilFire >= 0)
		m_countdownTimer = secondsUntilFire;
	else
		m_countdownTimer = m_secondsUntilFire;
}
//---------------------------------------------------------------------------
bool CountdownTrigger::update(float deltaT)
{
	if (!m_enabled)
		return false;

	m_countdownTimer -= deltaT;

	if (m_countdownTimer <= 0)
	{
		if (!m_repeat)
			m_countdownTimer = MathUtil::POSITIVE_INFINITY;
		else
			m_countdownTimer = m_secondsUntilFire;

		// fire the trigger
        Fire.raise(this);
	}

	return true;
}
