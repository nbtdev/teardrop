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

using namespace Teardrop::Integration;
using namespace Teardrop;

Havok::Animation::System::System()
{

}

Havok::Animation::System::~System()
{

}

void Havok::Animation::System::setAllocator(Allocator* pAlloc)
{
	assert(pAlloc);
	m_pAllocator = pAlloc;
}

Allocator* Havok::Animation::System::getAllocator()
{
	return m_pAllocator;
}

void Havok::Animation::System::getTypes(System::Type* typeArray, int& typeCount)
{
	if (typeCount < 1)
	{
		typeCount = 0;
		return; // TODO: throw?
	}

	typeArray[0] = System::SYSTEM_ANIMATION;
	typeCount = 1;
}

void Havok::Animation::System::initialize()
{
	// currently there really isn't anything to init for the animation
	// system, but as a hack to prevent stripping of the animation 
	// implementation classes, I need to mention them by name here...
	RigHavok cRig;
	AnimationHavok cAnim;
	cRig.destroy();
	cAnim.destroy();
}

void Havok::Animation::System::shutdown()
{
}

AnimationBlender* Havok::Animation::System::createBlender()
{
	// make one of the Havok variety...
	return TD_NEW AnimationBlenderHavok;
}