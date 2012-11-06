/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "AnimationBlender.h"
#include "Rig.h"
#include "Resource/ResourceManager.h"
#include "Util/Environment.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
AnimationBlender::AnimationBlender()
{
	m_hRig = INVALID_RESOURCE_HANDLE;
}
//---------------------------------------------------------------------------
AnimationBlender::~AnimationBlender()
{
}
//---------------------------------------------------------------------------
bool AnimationBlender::initialize(HResource hRig, const BlendGraph& /*graph*/)
{
	m_hRig = hRig;
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
	Rig* pRig = AutoResPtr<Rig>(m_hRig);
	if (!pRig)
		return 0;

	return pRig->getInstance(m_instanceIdx);
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
