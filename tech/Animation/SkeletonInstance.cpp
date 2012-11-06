/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Animation.h"
#include "Rig.h"
#include "SkeletonInstance.h"
#include "Math/Matrix44.h"
#include "Util/Environment.h"
#include "Util/SystemManager.h"
#include "Resource/ResourceManager.h"
#include "Memory/Allocators.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
SkeletonInstance::SkeletonInstance(Rig* pRig)
{
	Teardrop::System* pAnimSys = 
		Environment::get().pSystemMgr->getActiveSystem(Teardrop::System::SYSTEM_ANIMATION);

	m_boneCount = pRig->getBoneCount();
	m_pBoneCache = static_cast<Matrix44*>(
		pAnimSys->getAllocator()->AllocateAligned(
			sizeof(Matrix44) * m_boneCount,
			16 TD_ALLOC_SITE)
	);
}
//---------------------------------------------------------------------------
SkeletonInstance::~SkeletonInstance()
{
	Teardrop::System* pAnimSys = 
		Environment::get().pSystemMgr->getActiveSystem(Teardrop::System::SYSTEM_ANIMATION);

	pAnimSys->getAllocator()->DeallocateAligned(m_pBoneCache);
}
//---------------------------------------------------------------------------
const Matrix44* SkeletonInstance::getBoneCache() const
{
	return m_pBoneCache;
}
//---------------------------------------------------------------------------
Matrix44* SkeletonInstance::getBoneCache()
{
	return m_pBoneCache;
}
//---------------------------------------------------------------------------
size_t SkeletonInstance::getBoneCount() const
{
	return m_boneCount;
}
//---------------------------------------------------------------------------
size_t SkeletonInstance::getBoneIndexByName(const String& /*name*/) const
{
	return size_t(-1);
}
