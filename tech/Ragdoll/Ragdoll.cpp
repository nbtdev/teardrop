/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Ragdoll.h"
#include "RagdollSystem.h"
#include "Physics/Physics.h"
#include "Util/Environment.h"
#include "Util/Logger.h"
#include "Util/SystemManager.h"
#include "Memory/Allocators.h"
#include "Stream/Stream.h"
#include "Util/FourCC.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
DEFINE_SERIALIZABLE(Ragdoll);
//---------------------------------------------------------------------------
const FourCC& Ragdoll::RESOURCE_TYPE = FourCC('R','A','G','D');
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
bool Ragdoll::destroy()
{
	return true;
}
//---------------------------------------------------------------------------
bool Ragdoll::release()
{
	return true;
}
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
bool Ragdoll::load(Stream& strm)
{
	Teardrop::System* pSys = 
		Environment::get().pSystemMgr->getActiveSystem(Teardrop::System::SYSTEM_RAGDOLL);

	// load the whole stream and own the data once loaded
	if (m_pData)
	{
		pSys->getAllocator()->Deallocate(m_pData);
	}

	unsigned int len = (unsigned int)strm.length();
	m_pData = pSys->getAllocator()->AllocateAligned(len, 16 TD_ALLOC_SITE);
	strm.read(m_pData, len);

	return initialize(m_pData, len);
}
//---------------------------------------------------------------------------
bool Ragdoll::serialize(ResourceSerializer& /*ser*/)
{
	return false;
}
