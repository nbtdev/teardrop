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

#include "AnimationBlender.h"
#include "Rig.h"
#include "Util/Environment.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
AnimationBlender::AnimationBlender()
    //: m_hRig(nullptr)
{
}
//---------------------------------------------------------------------------
AnimationBlender::~AnimationBlender()
{
}
//---------------------------------------------------------------------------
bool AnimationBlender::initialize(Rig* rig, const BlendGraph& /*graph*/)
{
    m_pRig = rig;
	return true;
}
//---------------------------------------------------------------------------
bool AnimationBlender::destroy()
{
	return true;
}
//---------------------------------------------------------------------------
bool AnimationBlender::advance(float /*timeStep*/)
{
	return false;
}
//---------------------------------------------------------------------------
bool AnimationBlender::transitionToState(const String& /*stateName*/)
{
	return false;
}
//---------------------------------------------------------------------------
SkeletonInstance* AnimationBlender::getSkeletonInstance()
{
    if (!m_pRig)
		return 0;

    return m_pRig->getInstance(m_instanceIdx);
}
//---------------------------------------------------------------------------
void AnimationBlender::setAdditionalRotation(
	size_t /*boneIndex*/, const Quaternion& /*rot*/)
{
}
//---------------------------------------------------------------------------
size_t AnimationBlender::getNumMountPoints()
{
	return 0;
}
//---------------------------------------------------------------------------
const AnimationBlender::MountPointData* 
AnimationBlender::getMountPoint(size_t /*idx*/)
{
	return 0;
}
