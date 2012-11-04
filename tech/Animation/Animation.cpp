/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

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
-----------------------------------------------------------------------------
*/

#include "stdafx.h"
#include "Animation.h"
#include "Stream/Stream.h"
#include "Memory/Allocators.h"
#include "Util/FourCC.h"
#include "Util/Environment.h"
#include "Util/Logger.h"

#include "Havok/RigHavok.h"
#include "Havok/AnimationHavok.h"
#include "Havok/AnimationBlenderHavok.h"
#include <assert.h>

using namespace CoS;
//---------------------------------------------------------------------------
Allocator* Animation::s_pAllocator = 0; // USER MUST SET THIS
//---------------------------------------------------------------------------
const FourCC& Animation::RESOURCE_TYPE = FourCC('A','N','I','M');
//---------------------------------------------------------------------------
DEFINE_SERIALIZABLE(Animation);
//---------------------------------------------------------------------------
void Animation::setAllocator(Allocator* pAlloc)
{
	assert(pAlloc);
	s_pAllocator = pAlloc;
}
//---------------------------------------------------------------------------
Allocator* Animation::getAllocator()
{
	return s_pAllocator;
}
//---------------------------------------------------------------------------
bool Animation::initialize()
{
	// currently there really isn't anything to init for the animation
	// system, but as a hack to prevent stripping of the animation 
	// implementation classes, I need to mention them by name here...
	RigHavok cRig;
	AnimationHavok cAnim;
	cRig.destroy();
	cAnim.destroy();

	return true;
}
//---------------------------------------------------------------------------
bool Animation::shutdown()
{
	return true;
}
//---------------------------------------------------------------------------
AnimationBlender* Animation::createBlender()
{
	// make one of the Havok variety...
	return COS_NEW AnimationBlenderHavok;
}
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
	s_pAllocator->Deallocate(m_pData);
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
	// load the whole stream and own the data once loaded
	if (m_pData)
	{
		s_pAllocator->Deallocate(m_pData);
	}

	unsigned int len = (unsigned int)strm.length();
	m_pData = getAllocator()->AllocateAligned(len, 16 COS_ALLOC_SITE);
	strm.read(m_pData, len);

	return initialize(m_pData, len);
}
//---------------------------------------------------------------------------
bool Animation::serialize(ResourceSerializer& ser)
{
	// implemented by derived classes
	return false;
}