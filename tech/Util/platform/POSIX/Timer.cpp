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

