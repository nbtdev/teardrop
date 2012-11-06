/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "RagdollHavok.h"
#include "Serialization/ResourceSerializer.h"
#include "Util/Environment.h"
#include "Util/Logger.h"
#include "Util/_String.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
DEFINE_SERIALIZABLE(RagdollHavok);
//---------------------------------------------------------------------------
RagdollHavok::RagdollHavok()
{
	// SerialPointers init to zero in their ctors
}
//---------------------------------------------------------------------------
RagdollHavok::RagdollHavok(int i)
{
	UNREFERENCED_PARAMETER(i);
}
//---------------------------------------------------------------------------
RagdollHavok::~RagdollHavok()
{
}
//---------------------------------------------------------------------------
bool RagdollHavok::initialize(
	void* pData,
	unsigned int dataLen
	)
{
	return Ragdoll::initialize(pData, dataLen);
}
//---------------------------------------------------------------------------
bool RagdollHavok::initialize()
{
	release();

	return Ragdoll::initialize();
}
//---------------------------------------------------------------------------
bool RagdollHavok::release()
{
	if (m_pResource)
	{
		m_pResource->removeReference();
		m_pResource = 0;
	}

	return Ragdoll::release();
}
//---------------------------------------------------------------------------
void RagdollHavok::delayInit()
{
	if (m_pRagdollBodies)
		return;

	// check to see if we need to try to init
	m_pResource = hkSerializeUtil::load(m_pData, m_dataLen);

	hkRootLevelContainer* pRC = m_pResource->getContents<hkRootLevelContainer>();
	hkpPhysicsData* pData = static_cast<hkpPhysicsData*>(
		pRC->findObjectByType(hkpPhysicsDataClass.getName()));
	hkArray<hkpPhysicsSystem*> pSys;
	pSys = pData->getPhysicsSystems();
	m_pRagdollBodies = pSys[0];

	hkaSkeletonMapper *pLowToHigh, *pHighToLow;
	m_pInstance = static_cast<hkaRagdollInstance*>(
		pRC->findObjectByType(hkaRagdollInstanceClass.getName()));
	pLowToHigh = m_pLowToHighMapper = static_cast<hkaSkeletonMapper*>(
		pRC->findObjectByType(hkaSkeletonMapperClass.getName()));
	pHighToLow = m_pHighToLowMapper = static_cast<hkaSkeletonMapper*>(
		pRC->findObjectByType(hkaSkeletonMapperClass.getName(), m_pLowToHighMapper));

	if (m_pHighToLowMapper->m_mapping.m_skeletonA == m_pInstance->getSkeleton())
	{
		m_pLowToHighMapper = pHighToLow;
		m_pHighToLowMapper = pLowToHigh;
	}
}
//---------------------------------------------------------------------------
Ragdoll* RagdollHavok::clone()
{
	return 0;
}
//---------------------------------------------------------------------------
hkpPhysicsSystem* RagdollHavok::getPhysicsSystem()
{
	if (!m_pRagdollBodies)
		delayInit();

	return m_pRagdollBodies;
}
//---------------------------------------------------------------------------
hkaSkeletonMapper* RagdollHavok::getHighToLowSkeletonMapper()
{
	if (!m_pHighToLowMapper)
		delayInit();

	return m_pHighToLowMapper;
}
//---------------------------------------------------------------------------
hkaSkeletonMapper* RagdollHavok::getLowToHighSkeletonMapper()
{
	if (!m_pLowToHighMapper)
		delayInit();

	return m_pLowToHighMapper;
}
//---------------------------------------------------------------------------
hkaRagdollInstance* RagdollHavok::getRagdollInstance()
{
	if (!m_pInstance)
		delayInit();

	return m_pInstance;
}
//---------------------------------------------------------------------------
hkaRagdollInstance* RagdollHavok::cloneRagdollInstance()
{
	if (!m_pInstance)
		delayInit();

	if (m_pInstance)
		return m_pInstance->clone();
	else
		return 0;
}
//---------------------------------------------------------------------------
bool RagdollHavok::update(float deltaT, AnimationBlender* pAnimBlender)
{
	delayInit();
	return Ragdoll::update(deltaT, pAnimBlender);
}
//---------------------------------------------------------------------------
bool RagdollHavok::serialize(ResourceSerializer& ser)
{
	ser.start();

	// serialize the vtabl pointer entry
	ser.relocate(RagdollHavok::getSerialClass()->getId());

	// before we write the memory image, we want to make sure that the
	// "non-serializable" members are nulled (and then restored)
	hkpPhysicsSystem* pSys = m_pRagdollBodies;
	hkaSkeletonMapper* pHTLMapper = m_pHighToLowMapper;
	hkaSkeletonMapper* pLTHMapper = m_pLowToHighMapper;
	m_pRagdollBodies = 0;
	m_pHighToLowMapper = 0;
	m_pLowToHighMapper = 0;

	ser.relocate(this, sizeof(RagdollHavok));

	m_pRagdollBodies = pSys;
	m_pHighToLowMapper = pHTLMapper;
	m_pLowToHighMapper = pLTHMapper;

	// relocate the data serial pointer
	ser.relocate(m_pData);

	// and then relocate the actual data
	size_t sz = m_dataLen;
	ser.relocate(m_pData, sz);

	return ser.finalize();
}
