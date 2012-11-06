/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "RigHavok.h"
#include "SkeletonInstanceHavok.h"
#include "Util/_String.h"
#include "Memory/Memory.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
SkeletonInstanceHavok::SkeletonInstanceHavok(Rig* pRig)
: m_pInstance(0), SkeletonInstance(pRig)
{
	RigHavok* pRigH = static_cast<RigHavok*>(pRig);
	m_pInstance = new hkaAnimatedSkeleton(pRigH->getSkeleton());
}
//---------------------------------------------------------------------------
SkeletonInstanceHavok::~SkeletonInstanceHavok()
{
	delete m_pInstance;
}
//---------------------------------------------------------------------------
hkaAnimatedSkeleton* SkeletonInstanceHavok::getAnimatedSkeleton()
{
	return m_pInstance;
}
//---------------------------------------------------------------------------
size_t SkeletonInstanceHavok::getBoneIndexByName(const String& name) const
{
	// find the named bone in the skeleton, or return size_t(-1)
	if (m_pInstance)
	{
		const hkaSkeleton* pSkel = m_pInstance->getSkeleton();
		hkInt32 numBones = pSkel->m_bones.getSize();
		for (hkInt32 i=0; i<numBones; ++i)
		{
			if (name == pSkel->m_bones[i].m_name)
				return size_t(i);
		}
	}

	return size_t(-1);
}
