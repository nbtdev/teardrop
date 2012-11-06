/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "AnimationBlenderHavok.h"
#include "Animation/BlendGraph.h"
#include "RigHavok.h"
#include "AnimationHavok.h"
#include "SkeletonInstanceHavok.h"
#include "Memory/Memory.h"
#include "Resource/ResourceManager.h"
#include "Util/Environment.h"
#include "Math/Matrix44.h"
#include <assert.h>

using namespace Teardrop;
//---------------------------------------------------------------------------
AnimationBlenderHavok::AnimationBlenderHavok()
: AnimationBlender()
{
	m_pCurrentState = 0;
	m_transitionTime = 1;
	m_transitionDuration = 0;
}
//---------------------------------------------------------------------------
AnimationBlenderHavok::~AnimationBlenderHavok()
{
}
//---------------------------------------------------------------------------
bool AnimationBlenderHavok::initialize(HResource hRig, const BlendGraph& graph)
{
	if (!AnimationBlender::initialize(hRig, graph))
		return false;

	// initialize the blend state machine
	initializeAnimationStates(graph);

	return true;
}
//---------------------------------------------------------------------------
bool AnimationBlenderHavok::destroy()
{
	return true;
}
//---------------------------------------------------------------------------
void AnimationBlenderHavok::setAdditionalRotation(
	size_t boneIndex, const Quaternion& rot)
{
	AdditionalTransform xform;
	xform.boneIndex = boneIndex;
	xform.rot = rot;
	m_addlXforms.push_back(xform);
}
//---------------------------------------------------------------------------
bool AnimationBlenderHavok::advance(float timeStep)
{
	RigHavok* pRig = AutoResPtr<RigHavok>(m_hRig);
	SkeletonInstance* pSkeletonInstance = getSkeletonInstance();
	hkaAnimatedSkeleton* pInstH = 0;
	bool bHaveAllCurrentStateControls = true;

	if (!pRig)
	{
		// can't do anything until our rig has been loaded
		return false;
	}
	else
	{
		// then make sure we have a valid skeleton instance
		if (!pSkeletonInstance)
		{
			// try to make one
			m_instanceIdx = pRig->createInstance();
			pSkeletonInstance = getSkeletonInstance();

			// reconnect the current state's anim control(s) to the skeleton 
			// instance (other states will connect when transitioned to)
			if (pSkeletonInstance)
			{
				if (m_pCurrentState)
				{
					SkeletonInstanceHavok* pHInst = 
						static_cast<SkeletonInstanceHavok*>(pSkeletonInstance);

					State::Animations::iterator it = m_pCurrentState->animations.begin();
					while (it != m_pCurrentState->animations.end())
					{
						// add this to the Havok controller
						AnimationHavok* pAnim = AutoResPtr<AnimationHavok>(it->pAnim->hAnim);

						hkaAnimationControl* pHCtrl = pAnim->getControl(it->pAnim->hCtrl);
						if (!pHCtrl)
						{
							// this handle is no longer valid; try to make a new control
							it->pAnim->hCtrl = pAnim->createControl();
							pHCtrl = pAnim->getControl(it->pAnim->hCtrl);
						}

						if (pHCtrl)
						{
							// NOTE: this is basically caching a pointer we don't want cached, 
							// but it's how Havok works, so...we'll just have to check the validity
							// of animations and their controls at the sites they are used
							pHInst->getAnimatedSkeleton()->addAnimationControl(pHCtrl);

							hkaDefaultAnimationControl* pCtl = 
								static_cast<hkaDefaultAnimationControl*>(pHCtrl);
							pCtl->setPlaybackSpeed(it->speed);
						}
						else
							bHaveAllCurrentStateControls = false;

						++it;
					}
				}
			}
		}

		if (pSkeletonInstance)
		{
			SkeletonInstanceHavok* pInst = 
				static_cast<SkeletonInstanceHavok*>(pSkeletonInstance);

			pInstH = pInst->getAnimatedSkeleton();
			pInstH->setReferencePoseWeightThreshold( 0.05f );

			// fill in our mount points (local frames)
			const hkaSkeleton* pSkeleton = pInstH->getSkeleton();
			int nFrames = pSkeleton->m_localFrames.getSize();
			if (!m_localFrames.size() && nFrames)
			{
				m_localFrames.resize(nFrames);

				for (int i=0; i<nFrames; ++i)
				{
					const hkaSkeleton::LocalFrameOnBone& lf = 
						pSkeleton->m_localFrames[i];

					m_localFrames[i].pFrameData = &lf;
					m_localFrames[i].m_data.m_name = lf.m_localFrame->getName();

					const hkSimpleLocalFrame& simpleLF = (const hkSimpleLocalFrame&)*lf.m_localFrame;

					simpleLF.m_transform.get4x4ColumnMajor(m_localFrames[i].m_data.m_transform._m);
					m_localFrames[i].m_data.m_boneIndex = lf.m_boneIndex;
				}
			}
		}
	}

	// we can't proceed if either the rig or the state animations are not yet ready
	if (!pInstH || !bHaveAllCurrentStateControls)
	{
		return false;
	}

	// the primary reason that this step will fail is that not all animations or 
	// their controls are ready/valid
	if (!updateAnimationStates(timeStep))
		return false;

	// once we are good all around, we can advance
	pInstH->stepDeltaTime(timeStep);

	// sample the animation 
	hkaSkeleton* pSkeleton = pRig->getSkeleton();
	Matrix44* bones = pSkeletonInstance->getBoneCache();
	const int boneCount = pSkeleton->m_bones.getSize();
	hkaPose pose(pSkeleton);
	pInstH->sampleAndCombineAnimations( pose.accessUnsyncedPoseLocalSpace().begin(), pose.getFloatSlotValues().begin()  );

	// apply any pending additional transforms to the bones
	for (AdditionalTransforms::iterator it = m_addlXforms.begin();
		it != m_addlXforms.end(); ++it)
	{
		hkInt32 boneIndex(it->boneIndex);
		hkQuaternion& rot = (hkQuaternion&)it->rot;
		if (boneIndex < 0) // invalid bones are size_t(-1)
			continue;
		pose.accessBoneModelSpace(boneIndex, hkaPose::PROPAGATE).m_rotation.mul(rot);
	}

	m_addlXforms.clear();

	// get the bone transform in model space
	const hkArray<hkQsTransform>& poseInWorld = pose.getSyncedPoseModelSpace();

	// write 4x4's into our bone cache (matrix palette)
	for (int p=0; p < boneCount; p++)
		poseInWorld[p].get4x4ColumnMajor(bones[p]._m);

	// update the local frame transforms
	for (size_t i=0; i<m_localFrames.size(); ++i)
	{
		int idx = m_localFrames[i].pFrameData->m_boneIndex;

		hkTransform _xf;
		m_localFrames[i].pFrameData->m_localFrame->getLocalTransform(_xf);
		hkQsTransform xf, tmp;
		tmp.setFromTransform(_xf);
		xf.setMul(poseInWorld[idx], tmp);
		xf.get4x4ColumnMajor(m_localFrames[i].m_data.m_transform._m);
	}

	return true;
}
//---------------------------------------------------------------------------
void AnimationBlenderHavok::initializeAnimationStates(const BlendGraph& graph)
{
	m_animations.clear();
	m_states.clear();

	// first do the animations
	for (BlendGraph::Animations::const_iterator it = graph.m_animations.begin();
		it != graph.m_animations.end(); ++it)
	{
		AnimationBlenderHavok::Animation anim;
		anim.name = it->name;
		anim.hAnim = Environment::get().pResourceMgr->acquire(
			Teardrop::Animation::RESOURCE_TYPE, it->resource);

		// setting the pCtrl member will be done dynamically during update
		m_animations.push_back(anim);
	}

	typedef std::map<String, AnimationBlenderHavok::Animation*> AnimLUT;
	AnimLUT animLUT;
	for (size_t i=0; i<m_animations.size(); ++i)
	{
		animLUT[m_animations[i].name] = &(m_animations[i]);
	}

	// and then the states themselves
	for (BlendGraph::States::const_iterator it = graph.m_states.begin();
		it != graph.m_states.end(); ++it)
	{
		AnimationBlenderHavok::State state;
		state.name = it->name;

		for (size_t i=0; i<it->anims.size(); ++i)
		{
			const BlendGraph::AnimationWeight& weight = it->anims[i];
			AnimationBlenderHavok::State::BlendedAnim anim;
			anim.loop = weight.loop;
			anim.speed = weight.speed;
			anim.weight = weight.weight;
			anim.pAnim = animLUT.find(weight.pAnimation->name)->second;
			state.animations.push_back(anim);
		}

		m_states.push_back(state);
	}

	typedef std::map<String, AnimationBlenderHavok::State*> StateLUT;
	StateLUT stateLUT;
	for (size_t i=0; i<m_states.size(); ++i)
	{
		stateLUT[m_states[i].name] = &(m_states[i]);
	}

	// and finally the transitions
	for (BlendGraph::Transitions::const_iterator it = graph.m_transitions.begin();
		it != graph.m_transitions.end(); ++it)
	{
		StateLUT::iterator from = stateLUT.find(it->pFrom->name);
		StateLUT::iterator to = stateLUT.find(it->pTo->name);

		AnimationBlenderHavok::State::Transition trans;
		trans.duration = it->duration;
		trans.curve = (AnimationBlenderHavok::State::Transition::Curve)it->curve;
		trans.pTo = to->second;
		from->second->transitions.push_back(trans);
	}

	transitionToState(graph.m_initState);
}
//---------------------------------------------------------------------------
bool AnimationBlenderHavok::updateAnimationStates(float deltaT)
{
	RigHavok* pRig = AutoResPtr<RigHavok>(m_hRig);
	SkeletonInstance* pSkeletonInstance = getSkeletonInstance();
	hkaAnimatedSkeleton* pInstH = 0;

	if (!pRig)
	{
		// can't do anything until our rig has been loaded
		return false;
	}
	else
	{
		// then make sure we have a valid skeleton instance
		if (pSkeletonInstance)
		{
			SkeletonInstanceHavok* pInst = 
				static_cast<SkeletonInstanceHavok*>(pSkeletonInstance);

			pInstH = pInst->getAnimatedSkeleton();
			pInstH->setReferencePoseWeightThreshold( 0.05f );
		}
	}

	if (!pInstH)
	{
		return false;
	}

	float transProgress = 0;
	State* pNext = 0;
	bool allInitialized = true;
	if (m_nextStates.size())
	{
		pNext = m_nextStates.front();

		// try to get the anim controller that goes with the anims in this state
		for (size_t i=0; i<pNext->animations.size(); ++i)
		{
			State::BlendedAnim& anim = pNext->animations[i];
			AnimationHavok* pAnim = AutoResPtr<AnimationHavok>(anim.pAnim->hAnim);
			hkaAnimationControl* pCtrl = 0;
			if (pAnim)
				pCtrl = pAnim->getControl(anim.pAnim->hCtrl);

			if (pAnim && !pCtrl)
			{
				// create the havok animation controller for this anim
				// and add it to the Havok controller
				anim.pAnim->hCtrl = pAnim->createControl();
				pCtrl = pAnim->getControl(anim.pAnim->hCtrl);
				if (pCtrl)
				{
					// TODO: again, caching this pointer but there isn't much 
					// we can do about it, other than check every time we update
					// animations...
					pInstH->addAnimationControl(pCtrl);

					hkaDefaultAnimationControl* pDefCtl = 
						static_cast<hkaDefaultAnimationControl*>(pCtrl);
					pDefCtl->setPlaybackSpeed(anim.speed);
				}
			}

			allInitialized &= (pCtrl != 0);
		}

		if (!allInitialized)
		{
			return false; // because we're not ready yet
		}
	}

	// only do this during transition -- otherwise, there should only be a current state and 
	// it should be at full weight
	if (pNext)
	{
		m_transitionTime -= deltaT;
		if (m_transitionTime <= 0 || m_transitionDuration == 0)
		{
			if (m_pCurrentState)
			{
				for (size_t i=0; i<m_pCurrentState->animations.size(); ++i)
				{
					State::BlendedAnim& anim = m_pCurrentState->animations[i];
					AnimationHavok* pAnim = AutoResPtr<AnimationHavok>(anim.pAnim->hAnim);
					hkaDefaultAnimationControl* pCtrl = 0;
					if (pAnim)
						pCtrl = static_cast<hkaDefaultAnimationControl*>(pAnim->getControl(anim.pAnim->hCtrl));

					if (pCtrl)
					{
						pCtrl->setMasterWeight(0);
					}
				}
			}

			// pop the "next" state and reset the timer
			m_transitionTime = 1;
			m_nextStates.pop_front();
			m_pCurrentState = pNext;
			pNext = 0;

			for (size_t i=0; i<m_pCurrentState->animations.size(); ++i)
			{
				State::BlendedAnim& anim = m_pCurrentState->animations[i];
				AnimationHavok* pAnim = AutoResPtr<AnimationHavok>(anim.pAnim->hAnim);
				hkaDefaultAnimationControl* pCtrl = 0;
				if (pAnim)
					pCtrl = static_cast<hkaDefaultAnimationControl*>(pAnim->getControl(anim.pAnim->hCtrl));

				if (pCtrl)
				{
					pCtrl->setMasterWeight(anim.weight);
				}
			}
		}
		else
		{
			// update the state transition progress for weighting use below
			transProgress = 1 - (m_transitionTime / m_transitionDuration);

			// regardless, blend into the "next" one
			for (size_t i=0; i<pNext->animations.size(); ++i)
			{
				State::BlendedAnim& anim = pNext->animations[i];
				AnimationHavok* pAnim = AutoResPtr<AnimationHavok>(anim.pAnim->hAnim);
				hkaDefaultAnimationControl* pCtrl = 0;
				if (pAnim)
					pCtrl = static_cast<hkaDefaultAnimationControl*>(pAnim->getControl(anim.pAnim->hCtrl));

				if (pCtrl)
				{
					pCtrl->setMasterWeight(anim.weight * transProgress);
				}
			}

			if (m_pCurrentState) 
			{
				// then have to blend out of the current one
				for (size_t i=0; i<m_pCurrentState->animations.size(); ++i)
				{
					State::BlendedAnim& anim = m_pCurrentState->animations[i];
					AnimationHavok* pAnim = AutoResPtr<AnimationHavok>(anim.pAnim->hAnim);
					hkaDefaultAnimationControl* pCtrl = 0;
					if (pAnim)
						pCtrl = static_cast<hkaDefaultAnimationControl*>(pAnim->getControl(anim.pAnim->hCtrl));

					if (pCtrl)
					{
						pCtrl->setMasterWeight(anim.weight * (1 - transProgress));
					}
				}
			}
		}
	}

	// verify that the current state is all valid controls, and if not,
	// make them valid as needed
	bool bCurrentStateValid = true;
	if (m_pCurrentState) 
	{
		// then have to blend out of the current one
		for (size_t i=0; i<m_pCurrentState->animations.size(); ++i)
		{
			State::BlendedAnim& anim = m_pCurrentState->animations[i];
			AnimationHavok* pAnim = AutoResPtr<AnimationHavok>(anim.pAnim->hAnim);
			hkaAnimationControl* pCtrl = 0;
			if (pAnim)
			{
				pCtrl = pAnim->getControl(anim.pAnim->hCtrl);

				if (!pCtrl)
				{
					// attempt to create one
					anim.pAnim->hCtrl = pAnim->createControl();
					pCtrl = pAnim->getControl(anim.pAnim->hCtrl);

					// and add it to the current animated skeleton
					if (pCtrl)
						pInstH->addAnimationControl(pCtrl);
				}
			}

			bCurrentStateValid &= (pCtrl != 0);
		}
	}

	return bCurrentStateValid;
}
//---------------------------------------------------------------------------
bool AnimationBlenderHavok::transitionToState(const String& stateName)
{
	// make sure this transition has not already been scheduled
	if (m_nextStates.size())
	{
		if (m_nextStates.back()->name == stateName)
			return true;
	}

	// if we have a current state, ensure that this is a valid transition
	// from said state
	if (m_pCurrentState)
	{
		// check with the defined transitions
		for (size_t i=0; i<m_pCurrentState->transitions.size(); ++i)
		{
			if (stateName == m_pCurrentState->transitions[i].pTo->name)
			{
				m_nextStates.push_back(m_pCurrentState->transitions[i].pTo);
				m_transitionTime = (m_pCurrentState->transitions[i].curve == State::Transition::TRANSITION_STEP) ?
					0 : m_pCurrentState->transitions[i].duration;
				m_transitionDuration = m_transitionTime;
				return true;
			}
		}
	}
	else
	{
		// this is an initial state; search the list of defined states
		// for the named state
		for (size_t i=0; i<m_states.size(); ++i)
		{
			if (stateName == m_states[i].name)
			{
				m_nextStates.push_back(&m_states[i]);
				m_transitionTime = m_transitionDuration = 0;
				return true;
			}
		}
	}

	return false;
}
//---------------------------------------------------------------------------
const AnimationBlender::MountPointData*
AnimationBlenderHavok::getMountPoint(size_t idx)
{
	if (idx > m_localFrames.size())
		return 0;

	return &m_localFrames[idx].m_data;
}
