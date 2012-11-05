/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "AnimationSystem.h"
#include <assert.h>

#include "RigHavok.h"
#include "AnimationHavok.h"
#include "AnimationBlenderHavok.h"

using namespace Teardrop::Integration::Havok::Animation;

void System::setAllocator(CoS::Allocator* pAlloc)
{
	assert(pAlloc);
	m_pAllocator = pAlloc;
}

CoS::Allocator* System::getAllocator()
{
	return m_pAllocator;
}

void System::getTypes(Teardrop::System::Type* typeArray, int& typeCount)
{
	if (typeCount < 1)
	{
		typeCount = 0;
		return; // TODO: throw?
	}

	typeArray[0] = System::SYSTEM_ANIMATION;
	typeCount = 1;
}

void System::initialize()
{
	// currently there really isn't anything to init for the animation
	// system, but as a hack to prevent stripping of the animation 
	// implementation classes, I need to mention them by name here...
	CoS::RigHavok cRig;
	CoS::AnimationHavok cAnim;
	cRig.destroy();
	cAnim.destroy();
}

void System::shutdown()
{
}

CoS::AnimationBlender* System::createBlender()
{
	// make one of the Havok variety...
	return COS_NEW CoS::AnimationBlenderHavok;
}