/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Component_Animation.h"
#include "ZoneObject.h"
#include "Zone.h"
#include "Scene.h"
#include "Util/Hash.h"
#include "Util/Environment.h"
#include "Util/SystemManager.h"
#include "Animation/Rig.h"
#include "Animation/Animation.h"
#include "Animation/BlendGraph.h"
#include "Animation/SkeletonInstance.h"
#include "Animation/AnimationBlender.h"
#include "Memory/Memory.h"
#include "Memory/Allocators.h"
#include "Math/Matrix44.h"
#include <assert.h>

using namespace Teardrop;
//---------------------------------------------------------------------------
TD_CLASS_IMPL(AnimationComponent);
const String& AnimationComponent::SHORT_NAME = String("animation");
//---------------------------------------------------------------------------
AnimationComponent::AnimationComponent()
{
	m_pBlender = 0;
}
//---------------------------------------------------------------------------
AnimationComponent::~AnimationComponent()
{
}
//---------------------------------------------------------------------------
bool AnimationComponent::initialize()
{
	// add the rig if any
	if (getRigName().length())
	{
		String rigName(m_assetPath);
		rigName += "/";
		rigName += getRigName();

		addRig(rigName);
	}

	// and any defined animations
#if 0
	for (size_t i=0; i<getAnimations().getCount(); ++i)
	{
		String animName;
		if (getAnimationsAt(i, animName))
		{
			String animPath(m_assetPath);
			animPath += "/";
			animPath += animName;

			String name("anim");
			String num;
			StringUtil::toString(i, num);
			name += num;

			addAnimation(num, animPath);
		}
	}
#endif

	AnimationSystem* pAnimSys = 
		static_cast<AnimationSystem*>(Environment::get().pSystemMgr->getActiveSystem(Teardrop::System::SYSTEM_ANIMATION));
	m_pBlender = pAnimSys->createBlender();

	// load the blend graph definition
	BlendGraph graph;
	String configPath(m_assetPath);
	configPath += "/";
	configPath += getBlendConfigName();
#if 0
    if (!BlendGraph::load(configPath, graph))
		return false;

	m_pBlender->initialize(m_hRig, graph);

	// add all of the animations that have been added to date
	for(AnimationLUT::iterator it = m_lut.begin(); it != m_lut.end(); ++it)
	{
		m_pBlender->addAnimation(it->second);
	}
#endif

	return true;
}
//---------------------------------------------------------------------------
bool AnimationComponent::destroy()
{
	if (m_pBlender)
	{
		delete m_pBlender;
		m_pBlender = 0;
	}

	return true;
}
//---------------------------------------------------------------------------
bool AnimationComponent::update(float deltaT)
{
	if (m_pBlender)
		return m_pBlender->advance(deltaT);

	return true;
}
#if 0
//---------------------------------------------------------------------------
bool AnimationComponent::addAnimation(const char* name, const char* assetPath)
{
	// if there is already an animation with this name, release it first
	String sName(name);

	AnimationLUT::iterator it = m_lut.find(sName);
	if (it != m_lut.end())
	{
		Environment::get().pResourceMgr->release(it->second);
	}

	HResource hAnim = Environment::get().pResourceMgr->acquire(
			Animation::RESOURCE_TYPE, assetPath);
	m_lut[sName] = hAnim;

	if (m_pBlender)
	{
		m_pBlender->addAnimation(hAnim);
	}

	return true;
}
#endif
//---------------------------------------------------------------------------
bool AnimationComponent::addRig(const char* assetPath)
{
#if 0
    if (m_hRig != INVALID_RESOURCE_HANDLE)
	{
		// release existing one first
		Environment::get().pResourceMgr->release(m_hRig);
	}

	m_hRig = Environment::get().pResourceMgr->acquire(
		Rig::RESOURCE_TYPE, assetPath);
#endif
	return true;
}
//---------------------------------------------------------------------------
const Matrix44* AnimationComponent::getBoneCache() const
{
	if (m_pBlender)
	{
		SkeletonInstance* pInst = m_pBlender->getSkeletonInstance();
		if (pInst)
			return pInst->getBoneCache();
	}

	return 0;
}
//---------------------------------------------------------------------------
size_t AnimationComponent::getNumBones() const
{
	if (m_pBlender)
	{
		SkeletonInstance* pInst = m_pBlender->getSkeletonInstance();
		if (pInst)
			return pInst->getBoneCount();
	}

	return 0;
}
//---------------------------------------------------------------------------
AnimationBlender* AnimationComponent::getBlender()
{
	return m_pBlender;
}
//---------------------------------------------------------------------------
size_t AnimationComponent::getBoneIndexByName(const String& name)
{
	if (m_pBlender)
	{
		SkeletonInstance* pInst = m_pBlender->getSkeletonInstance();
		if (pInst)
			return pInst->getBoneIndexByName(name);
	}

	return size_t(-1);
}
//---------------------------------------------------------------------------
void AnimationComponent::setAdditionalBoneRotationByIndex(
	size_t idx, const Quaternion& rot)
{
	if (m_pBlender)
		m_pBlender->setAdditionalRotation(idx, rot);
}
//---------------------------------------------------------------------------
void AnimationComponent::transitionToState(const String& state)
{
	// note that the blender will ignore invalid transitions (legal transitions
	// are defined in the data)
	if (m_pBlender)
		m_pBlender->transitionToState(state);
}
