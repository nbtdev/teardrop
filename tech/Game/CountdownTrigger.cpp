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
