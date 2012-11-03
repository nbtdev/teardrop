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
#include "AnimationHavok.h"
#include "Serialization/ResourceSerializer.h"

using namespace CoS;
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
