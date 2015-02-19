/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Animation.h"
#include "Rig.h"
#include "Stream/Stream.h"
#include "Memory/Allocators.h"
#include "Util/Environment.h"
#include "Util/SystemManager.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
Rig::Rig()
{
	m_pData = 0;
	m_dataLen = 0;
}
//---------------------------------------------------------------------------
Rig::~Rig()
{
}
//---------------------------------------------------------------------------
bool Rig::initialize(void* pData, unsigned int dataLen)
{
	m_dataLen = dataLen;
	m_pData = pData;

	return true;
}
//---------------------------------------------------------------------------
bool Rig::destroy()
{
	Teardrop::System* pAnimSys = 
		Environment::get().pSystemMgr->getActiveSystem(Teardrop::System::SYSTEM_ANIMATION);

	pAnimSys->getAllocator()->Deallocate(m_pData);
	m_pData = 0;

	return true;
}
//---------------------------------------------------------------------------
size_t Rig::createInstance()
{
	return size_t(INVALID_SKELETON_INSTANCE_HANDLE);
}
//---------------------------------------------------------------------------
SkeletonInstance* Rig::getInstance(size_t)
{
	return 0;
}
//---------------------------------------------------------------------------
bool Rig::release()
{
	return true;
}
//---------------------------------------------------------------------------
Bone* Rig::getBone(const char* /*name*/)
{
	return 0;
}
//---------------------------------------------------------------------------
size_t Rig::getBoneCount()
{
	return 0;
}
//---------------------------------------------------------------------------
Bone* Rig::getBone(size_t /*index*/)
{
	return 0;
}
//---------------------------------------------------------------------------
bool Rig::load(Stream& strm)
{
	Teardrop::System* pAnimSys = 
		Environment::get().pSystemMgr->getActiveSystem(Teardrop::System::SYSTEM_ANIMATION);

	// load the whole stream and own the data once loaded
	if (m_pData)
	{
		pAnimSys->getAllocator()->Deallocate(m_pData);
	}

	unsigned int len = (unsigned int)strm.length();
	m_pData = pAnimSys->getAllocator()->AllocateAligned(len, 16 TD_ALLOC_SITE);
	strm.read(m_pData, len);

	return initialize(m_pData, len);
}
//---------------------------------------------------------------------------
bool Rig::serialize(ResourceSerializer& /*ser*/)
{
	// implemented by derived classes
	return false;
}
