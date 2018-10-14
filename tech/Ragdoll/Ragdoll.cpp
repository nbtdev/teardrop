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

#include "Ragdoll.h"
#include "RagdollSystem.h"
#include "Physics/Physics.h"
#include "Util/Environment.h"
#include "Util/Logger.h"
#include "Util/SystemManager.h"
#include "Memory/Allocators.h"
#include "Stream/Stream.h"
//#include "Util/FourCC.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
//DEFINE_SERIALIZABLE(Ragdoll);
//---------------------------------------------------------------------------
//const FourCC& Ragdoll::RESOURCE_TYPE = FourCC('R','A','G','D');
//---------------------------------------------------------------------------
Ragdoll::Ragdoll()
{
	m_pData = 0;
	m_dataLen = 0;
}
//---------------------------------------------------------------------------
Ragdoll::Ragdoll(int /*i*/)
{
}
//---------------------------------------------------------------------------
Ragdoll::~Ragdoll()
{
}
//---------------------------------------------------------------------------
bool Ragdoll::initialize(
	void* pData,
	unsigned int dataLen
	)
{
	m_pData = pData;
	m_dataLen = dataLen;
	return true;
}
//---------------------------------------------------------------------------
bool Ragdoll::initialize()
{
	return true;
}
//---------------------------------------------------------------------------
//bool Ragdoll::destroy()
//{
//	return true;
//}
//---------------------------------------------------------------------------
//bool Ragdoll::release()
//{
//	return true;
//}
//---------------------------------------------------------------------------
bool Ragdoll::update(float /*deltaT*/, AnimationBlender* /*pAnimBlender*/)
{
	return true;
}
//---------------------------------------------------------------------------
Ragdoll* Ragdoll::clone()
{
	// revisit this when we have a solution for cloning havok rigid bodies
	return 0;
}
//---------------------------------------------------------------------------
//bool Ragdoll::load(Stream& strm)
//{
//	Teardrop::System* pSys =
//		Environment::get().pSystemMgr->getActiveSystem(Teardrop::System::SYSTEM_RAGDOLL);

//	// load the whole stream and own the data once loaded
//	if (m_pData)
//	{
//		pSys->getAllocator()->Deallocate(m_pData);
//	}

//	unsigned int len = (unsigned int)strm.length();
//	m_pData = pSys->getAllocator()->AllocateAligned(len, 16 TD_ALLOC_SITE);
//	strm.read(m_pData, len);

//	return initialize(m_pData, len);
//}
//---------------------------------------------------------------------------
//bool Ragdoll::serialize(ResourceSerializer& /*ser*/)
//{
//	return false;
//}
