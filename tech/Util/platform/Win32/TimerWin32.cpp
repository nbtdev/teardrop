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

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Timer.h"
#include "Memory/Memory.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
static const int RESAMPLE_COUNT = 500;
//---------------------------------------------------------------------------
struct Timer::Impl
{
	LARGE_INTEGER currentTime;
	LARGE_INTEGER lastTime;
	int updates;
	LARGE_INTEGER freq;

	TD_DECLARE_ALLOCATOR();
};
//---------------------------------------------------------------------------
Timer::Timer()
{
	SetThreadAffinityMask(GetCurrentThread(), 1);
	m_pImpl = TD_NEW Impl;
	reset();
}
//---------------------------------------------------------------------------
Timer::~Timer()
{
	delete m_pImpl;
}
//---------------------------------------------------------------------------
float Timer::getCurrentTime() const
{
	return (float)m_pImpl->currentTime.QuadPart / (float)m_pImpl->freq.QuadPart;
}
//---------------------------------------------------------------------------
float Timer::saveElapsedTime()
{
	m_elapsed = (float)(m_pImpl->currentTime.QuadPart
		- m_pImpl->lastTime.QuadPart) / (float)m_pImpl->freq.QuadPart;
	m_pImpl->lastTime = m_pImpl->currentTime;
	return m_elapsed;
}
//---------------------------------------------------------------------------
float Timer::reset()
{
	float rtn = (float)m_pImpl->currentTime.QuadPart / 1000.f;
	m_pImpl->currentTime.QuadPart = 0;
	m_pImpl->lastTime.QuadPart = 0;
	m_pImpl->updates = 500;
	m_pImpl->freq.QuadPart = 0;
	m_elapsed = 0;
	return rtn;
}
//---------------------------------------------------------------------------
void Timer::update()
{
	++m_pImpl->updates;
	if (m_pImpl->updates > RESAMPLE_COUNT)
	{
		QueryPerformanceFrequency(&m_pImpl->freq);
		m_pImpl->updates = 0;
	}

	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);
	saveElapsedTime();
	m_pImpl->currentTime = time;
}
