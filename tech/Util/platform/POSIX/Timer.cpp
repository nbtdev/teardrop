/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Util/Timer.h"
#include "Memory/Memory.h"
#include <chrono>

using namespace Teardrop;
//---------------------------------------------------------------------------
struct Timer::Impl
{
    std::chrono::time_point<std::chrono::high_resolution_clock> mLastTime;

    TD_DECLARE_ALLOCATOR();
};
//---------------------------------------------------------------------------
Timer::Timer()
{
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
    std::chrono::time_point<std::chrono::high_resolution_clock> now =
            std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> sinceEpoch = now.time_since_epoch();
    return sinceEpoch.count();
}
//---------------------------------------------------------------------------
float Timer::saveElapsedTime()
{
    std::chrono::time_point<std::chrono::high_resolution_clock> now =
            std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsed = now - m_pImpl->mLastTime;
    m_pImpl->mLastTime = now;
    m_elapsed = elapsed.count();

    return m_elapsed;
}
//---------------------------------------------------------------------------
float Timer::reset()
{
    m_pImpl->mLastTime = std::chrono::high_resolution_clock::now();
    m_elapsed = 0;
    return 0;
}
//---------------------------------------------------------------------------
void Timer::update()
{
}

