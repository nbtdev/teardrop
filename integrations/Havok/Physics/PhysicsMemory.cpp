/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "Util/System.h"
#include "Util/SystemManager.h"
#include "Util/Environment.h"
#include "PhysicsMemory.h"
#include "Memory/Allocators.h"
#include <new.h>

using namespace Teardrop;
//---------------------------------------------------------------------------
PhysicsMemory::PhysicsMemory()
{
}
//---------------------------------------------------------------------------
PhysicsMemory::~PhysicsMemory()
{
}
//---------------------------------------------------------------------------
void* PhysicsMemory::blockAlloc(int numBytes)
{
	Teardrop::System* pSys = 
		Environment::get().pSystemMgr->getActiveSystem(Teardrop::System::SYSTEM_PHYSICS);

	return pSys->getAllocator()->AllocateAligned(numBytes, 16 TD_ALLOC_SITE);
}
//---------------------------------------------------------------------------
void PhysicsMemory::blockFree(void* p, int numBytes)
{
	Teardrop::System* pSys = 
		Environment::get().pSystemMgr->getActiveSystem(Teardrop::System::SYSTEM_PHYSICS);

	pSys->getAllocator()->DeallocateAligned(p);
}
//---------------------------------------------------------------------------
void PhysicsMemory::getMemoryStatistics(MemoryStatistics& u)
{
}
//---------------------------------------------------------------------------
int PhysicsMemory::getAllocatedSize(const void* obj, int nbytes)
{
	return nbytes;
}
