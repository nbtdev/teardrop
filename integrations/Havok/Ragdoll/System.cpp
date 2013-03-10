/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "System.h"
#include "RagdollHavok.h"
#include "RagdollControllerHavok.h"

using namespace Teardrop::Integration;
using namespace Teardrop;
//---------------------------------------------------------------------------
Havok::Ragdoll::System::System()
{
}
//---------------------------------------------------------------------------
Havok::Ragdoll::System::~System()
{
}
//---------------------------------------------------------------------------
void Havok::Ragdoll::System::setAllocator(Allocator* pAlloc)
{
	m_pAllocator = pAlloc;
}
//---------------------------------------------------------------------------
Allocator* Havok::Ragdoll::System::getAllocator()
{
	return m_pAllocator;
}
//---------------------------------------------------------------------------
void Havok::Ragdoll::System::getTypes(System::Type* typeArray, int& typeCount)
{
	if (typeCount < 1)
	{
		typeCount = 0;
		return; // TODO: throw?
	}

	typeArray[0] = System::SYSTEM_RAGDOLL;
	typeCount = 1;
}
//---------------------------------------------------------------------------
void Havok::Ragdoll::System::initialize()
{
	// static-init hackery
	RagdollHavok cRagdollHavok;
	RagdollControllerHavok cRagdollControllerHavok;
	cRagdollHavok.destroy();
	cRagdollControllerHavok.destroy();
}
//---------------------------------------------------------------------------
void Havok::Ragdoll::System::shutdown()
{
}
//---------------------------------------------------------------------------
RagdollController* Havok::Ragdoll::System::createController()
{
	return TD_NEW RagdollControllerHavok;
}
//---------------------------------------------------------------------------
void Havok::Ragdoll::System::destroyController(RagdollController* pController)
{
	delete pController;
}
