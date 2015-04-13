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
