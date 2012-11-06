/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "AnimationHavok.h"
#include "Serialization/ResourceSerializer.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
DEFINE_SERIALIZABLE(AnimationHavok);
//---------------------------------------------------------------------------
AnimationHavok::AnimationHavok()
{
}
//---------------------------------------------------------------------------
AnimationHavok::~AnimationHavok()
{
}
//---------------------------------------------------------------------------
bool AnimationHavok::initialize(void* pData, unsigned int dataLen)
{
	if (!Animation::initialize(pData, dataLen))
	{
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
bool AnimationHavok::destroy()
{
	for (size_t i=0; i<m_controls.size(); ++i)
	{
		hkaAnimationControl* pCtrl = 
			(hkaAnimationControl*)m_controls[i];

		// force the control to die, and also notify its listeners that is
		// is terminating (thus detaching it from any skeleton instances)
		while (pCtrl->getReferenceCount())
			pCtrl->removeReference();
	}

	m_controls.clear();

	if (m_pResource)
	{
		m_pResource->removeReference();
		m_pResource = 0;
	}

	// m_pBinding is a weak pointer to an object in the resource data

	return true;
}
//---------------------------------------------------------------------------
bool AnimationHavok::release()
{
	return destroy();
}
//---------------------------------------------------------------------------
hkaAnimationControl* AnimationHavok::getControl(size_t idx)
{
	if (idx < m_controls.size())
		return m_controls[idx];

	return 0;
}
//---------------------------------------------------------------------------
size_t AnimationHavok::createControl()
{
	// if this pointer has not been set yet, then we haven't been fully
	// initialized yet
	if (!m_pBinding)
	{
		delayInit();
	}

	if (m_pBinding)
	{
		size_t idx = m_controls.size();
		hkaDefaultAnimationControl* pCtrl = 
			new hkaDefaultAnimationControl(m_pBinding);
		m_controls.push_back(pCtrl);
		return idx;
	}

	return size_t(INVALID_ANIMATION_CONTROL_HANDLE);
}
//---------------------------------------------------------------------------
void AnimationHavok::delayInit()
{
	if (!m_pBinding)
	{
		m_pResource = hkSerializeUtil::load(m_pData, m_dataLen);
		hkRootLevelContainer* pRC = m_pResource->getContents<hkRootLevelContainer>();
		hkaAnimationContainer* pAC = static_cast<hkaAnimationContainer*>(
			pRC->findObjectByType(hkaAnimationContainerClass.getName()));

		if (pAC)
		{
			if (pAC->m_animations.getSize())
			{
				// just the first (and should be, only) one for now
				m_pBinding = pAC->m_bindings[0];
			}
		}
	}
}
//---------------------------------------------------------------------------
bool AnimationHavok::serialize(ResourceSerializer& ser)
{
	ser.start();


	// serialize the vtabl pointer entry
	ser.relocate(AnimationHavok::getSerialClass()->getId());

	// before we write the memory image, we want to make sure that the
	// "non-serializable" members are nulled (and then restored)
	hkaAnimationBinding* pBinding = m_pBinding;
	SerialPointerArray<hkaAnimationControl> tmp = m_controls;
	m_pBinding = 0;
	m_controls.first = 0;
	m_controls.sz = 0;

	ser.relocate(this, sizeof(AnimationHavok));

	m_controls = tmp;
	m_pBinding = pBinding;

	// relocate the data serial pointer
	ser.relocate(m_pData);

	// and then relocate the actual data
	size_t sz = m_dataLen;
	ser.relocate(m_pData, sz);

	return ser.finalize();
}
