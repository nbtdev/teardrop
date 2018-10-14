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
