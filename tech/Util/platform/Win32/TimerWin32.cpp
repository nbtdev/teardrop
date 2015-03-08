/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
