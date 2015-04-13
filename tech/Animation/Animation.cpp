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

#include "Animation.h"
#include "Memory/Allocators.h"
#include "Util/Environment.h"
#include "Util/Logger.h"
//#include "Util/System.h"
//#include "Util/SystemManager.h"
#include <assert.h>

using namespace Teardrop;
//---------------------------------------------------------------------------
Animation::Animation()
{
	m_pData = 0;
	m_dataLen = 0;
}
//---------------------------------------------------------------------------
Animation::~Animation()
{
}
//---------------------------------------------------------------------------
bool Animation::initialize(void* pData, unsigned int dataLen)
{
	m_dataLen = dataLen;
	m_pData = pData;

	return true;
}
//---------------------------------------------------------------------------
bool Animation::destroy()
{
	Teardrop::System* pAnimSys = 
		Environment::get().pSystemMgr->getActiveSystem(Teardrop::System::SYSTEM_ANIMATION);

	pAnimSys->getAllocator()->Deallocate(m_pData);
	m_pData = 0;

	return true;
}
//---------------------------------------------------------------------------
bool Animation::release()
{
	return true;
}
////---------------------------------------------------------------------------
//bool Animation::load(Stream& strm)
//{
//	Teardrop::System* pAnimSys =
//		Environment::get().pSystemMgr->getActiveSystem(Teardrop::System::SYSTEM_ANIMATION);

//	// load the whole stream and own the data once loaded
//	if (m_pData)
//	{
//		pAnimSys->getAllocator()->Deallocate(m_pData);
//	}

//	unsigned int len = (unsigned int)strm.length();
//	m_pData = pAnimSys->getAllocator()->AllocateAligned(len, 16 TD_ALLOC_SITE);
//	strm.read(m_pData, len);

//	return initialize(m_pData, len);
//}
////---------------------------------------------------------------------------
//bool Animation::serialize(ResourceSerializer& /*ser*/)
//{
//	// implemented by derived classes
//	return false;
//}
