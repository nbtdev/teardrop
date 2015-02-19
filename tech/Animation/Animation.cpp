/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Animation.h"
#include "Stream/Stream.h"
#include "Memory/Allocators.h"
#include "Util/Environment.h"
#include "Util/Logger.h"
#include "Util/System.h"
#include "Util/SystemManager.h"
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
//---------------------------------------------------------------------------
bool Animation::load(Stream& strm)
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
bool Animation::serialize(ResourceSerializer& /*ser*/)
{
	// implemented by derived classes
	return false;
}
