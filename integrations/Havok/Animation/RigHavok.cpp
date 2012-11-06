/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "RigHavok.h"
#include "BoneHavok.h"
#include "SkeletonInstanceHavok.h"
#include "Animation/Animation/Rig/hkaSkeleton.h"
#include "Animation/Animation/Rig/hkaBone.h"
#include "Serialization/ResourceSerializer.h"
#include "Memory/Memory.h"
#include <assert.h>

using namespace Teardrop;
//---------------------------------------------------------------------------
DEFINE_SERIALIZABLE(RigHavok);
//---------------------------------------------------------------------------
RigHavok::RigHavok()
{
}
//---------------------------------------------------------------------------
RigHavok::~RigHavok()
{
}
//---------------------------------------------------------------------------
bool RigHavok::initialize(void* pData, unsigned int dataLen)
{
	if (!Rig::initialize(pData, dataLen))
	{
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
bool RigHavok::release()
{
	// todo: check refcount, anyone?
	return destroy();
}
//---------------------------------------------------------------------------
bool RigHavok::destroy()
{
	if (m_pBones)
	{
		delete [] m_pBones;
		m_pBones = 0;
	}

	if (m_pResource)
	{
		m_pResource->removeReference();
		m_pResource = 0;
	}

	for (size_t i=0; i<m_instances.size(); ++i)
	{
		// remove all animation controls first
		hkaAnimatedSkeleton* pSkel = m_instances[i]->getAnimatedSkeleton();
		for (int j=0; j<pSkel->getNumAnimationControls(); ++j)
			pSkel->removeAnimationControl(pSkel->getAnimationControl(i));

		delete m_instances[i];
	}

	m_instances.clear();

	return true;
}
//---------------------------------------------------------------------------
size_t RigHavok::createInstance()
{
	delayInit(); // just in case

	if (!m_pRig)
		return size_t(INVALID_SKELETON_INSTANCE_HANDLE);

	SkeletonInstanceHavok* pInst = TD_NEW SkeletonInstanceHavok(this);
	size_t idx = m_instances.size();
	m_instances.push_back(pInst);
	return idx;
}
//---------------------------------------------------------------------------
SkeletonInstance* RigHavok::getInstance(size_t idx)
{
	if (idx >= m_instances.size())
		return 0;

	return m_instances[idx];
}
//---------------------------------------------------------------------------
hkaSkeleton* RigHavok::getSkeleton()
{
	delayInit(); // just in case
	return m_pRig;
}
//---------------------------------------------------------------------------
size_t RigHavok::getBoneCount()
{
	delayInit();
	if (m_pRig)
	{
		return (size_t)m_pRig->m_bones.getSize();
	}

	return 0;
}
//---------------------------------------------------------------------------
Bone* RigHavok::getBone(const char* name)
{
	if (!m_pRig || !m_pBones)
	{
		delayInit();
	}

	for (int i=0; i<m_pRig->m_bones.getSize(); ++i)
	{
		if (!strcmp(m_pRig->m_bones[i].m_name, name))
		{
			return &m_pBones[i];
		}
	}

	return 0;
}
//---------------------------------------------------------------------------
Bone* RigHavok::getBone(size_t index)
{
	if (!m_pRig || !m_pBones)
	{
		delayInit();
	}

	assert((int)index < m_pRig->m_bones.getSize());
	if ((int)index >= m_pRig->m_bones.getSize())
	{
		return 0;
	}

	return &m_pBones[index];
}
//---------------------------------------------------------------------------
void RigHavok::delayInit()
{
	if (!m_pRig)
	{
		m_pResource = hkSerializeUtil::load(m_pData, m_dataLen);
		hkRootLevelContainer* pRC = m_pResource->getContents<hkRootLevelContainer>();
		hkaAnimationContainer* pAC = static_cast<hkaAnimationContainer*>(
			pRC->findObjectByType(hkaAnimationContainerClass.getName()));

		if (pAC)
		{
			if (pAC->m_skeletons.getSize())
			{
				// just the first (and should be, only) one for now
				m_pRig = pAC->m_skeletons[0];
			}
		}
	}

	if (!m_pRig)
	{
		return;
	}

	if (!m_pBones)
	{
		// assign the bones pointer a dynamic array of BoneHavok instances,
		// that are constructed from the Havok skeleton bones
		m_pBones = TD_NEW BoneHavok[m_pRig->m_bones.getSize()];
		for (int i=0; i<m_pRig->m_bones.getSize(); ++i)
		{
			m_pBones[i].initialize(m_pRig, &m_pRig->m_bones[i]);
		}
	}
}
//---------------------------------------------------------------------------
bool RigHavok::serialize(ResourceSerializer& ser)
{
	ser.start();

	// serialize the vtabl pointer entry
	ser.relocate(RigHavok::getSerialClass()->getId());

	// before we write the memory image, we want to make sure that the
	// "non-serializable" members are nulled (and then restored)
	hkaSkeleton* pRig = m_pRig;
	BoneHavok* pBones = m_pBones;
	SerialPointerArray<SkeletonInstanceHavok> pInst = m_instances;

	m_pRig = 0;
	m_pBones = 0;
	m_instances.first = 0;
	m_instances.sz = 0;

	ser.relocate(this, sizeof(RigHavok));

	m_pRig = pRig;
	m_pBones = pBones;
	m_instances.first = pInst.first;
	m_instances.sz = pInst.sz;

	// relocate the data serial pointer
	ser.relocate(m_pData);

	// and then relocate the actual data
	size_t sz = m_dataLen;
	ser.relocate(m_pData, sz);

	return ser.finalize();
}
