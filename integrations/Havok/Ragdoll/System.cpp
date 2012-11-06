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

using namespace Teardrop::Integration::Havok::Ragdoll;
using namespace CoS;
//---------------------------------------------------------------------------
void System::setAllocator(Allocator* pAlloc)
{
	m_pAllocator = pAlloc;
}
//---------------------------------------------------------------------------
CoS::Allocator* System::getAllocator()
{
	return m_pAllocator;
}
//---------------------------------------------------------------------------
void System::getTypes(Teardrop::System::Type* typeArray, int& typeCount)
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
void System::initialize()
{
	// static-init hackery
	RagdollHavok cRagdollHavok;
	RagdollControllerHavok cRagdollControllerHavok;
	cRagdollHavok.destroy();
	cRagdollControllerHavok.destroy();
}
//---------------------------------------------------------------------------
void System::shutdown()
{
}
//---------------------------------------------------------------------------
CoS::RagdollController* System::createController()
{
	return COS_NEW RagdollControllerHavok;
}
//---------------------------------------------------------------------------
void System::destroyController(CoS::RagdollController* pController)
{
	delete pController;
}