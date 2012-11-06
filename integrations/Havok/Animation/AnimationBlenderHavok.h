/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(ANIMATIONBLENDERHAVOK_INCLUDED)
#define ANIMATIONBLENDERHAVOK_INCLUDED

#include "Animation/AnimationBlender.h"
#include "RigHavok.h"
#include "AnimationHavok.h"
#include "Math/Quaternion.h"
#include "Math/Matrix44.h"
#include "Util/_String.h"
#include <list>
#include <vector>
#include <map>

namespace Teardrop
{
	class AnimationBlenderHavok : public AnimationBlender
	{
		// don't ask...
		struct AdditionalTransform
		{
			size_t boneIndex;
			Quaternion rot;
		};
		typedef std::list<AdditionalTransform> AdditionalTransforms;
		AdditionalTransforms m_addlXforms;

	public:
		//! normal c'tor (cannot fail)
		AnimationBlenderHavok();
		//! d'tor (cannot fail)
		~AnimationBlenderHavok();

		bool initialize(HResource hRig, const BlendGraph& graph);
		bool destroy();
		bool advance(float timeStep);
		bool transitionToState(const String& stateName);

		void setAdditionalRotation(size_t boneIndex, const Quaternion& rot);

		size_t getNumMountPoints() { return m_localFrames.size(); }
		const MountPointData* getMountPoint(size_t idx);

		TD_DECLARE_ALLOCATOR();

	protected:
		struct LocalFrame
		{
			const hkaSkeleton::LocalFrameOnBone* pFrameData;
			MountPointData m_data;
		};
		typedef std::vector<LocalFrame> LocalFrames;
		LocalFrames m_localFrames;

		void initializeAnimationStates(const BlendGraph& graph);
		bool updateAnimationStates(float deltaT);

		struct Animation
		{
			//hkaDefaultAnimationControl* pCtrl;
			size_t hCtrl;
			HResource hAnim;
			String name;

			Animation() 
			{
				hAnim = INVALID_RESOURCE_HANDLE; 
				hCtrl = size_t(AnimationHavok::INVALID_ANIMATION_CONTROL_HANDLE);
			}
		};
		typedef std::vector<Animation> Animations;
		Animations m_animations;

		struct State
		{
			String name;
			struct BlendedAnim
			{
				Animation* pAnim;
				float weight;
				float speed;
				int loop;

				BlendedAnim() 
				{
					pAnim = 0;
					weight = 1;
					speed = 1;
					loop = 0;
				}
			};
			typedef std::vector<BlendedAnim> Animations;
			Animations animations;

			struct Transition
			{
				enum Curve
				{
					TRANSITION_STEP,
					TRANSITION_LINEAR,
					TRANSITION_CUBIC,
				};

				State* pTo;
				float duration;
				Curve curve;

				Transition() { duration = 0; pTo = 0; curve = TRANSITION_LINEAR; }
			};
			typedef std::vector<Transition> Transitions;
			Transitions transitions;
		};
		typedef std::vector<State> States;
		States m_states;

		State* m_pCurrentState;
		typedef std::list<State*> TransitionQueue;
		TransitionQueue m_nextStates;
		float m_transitionTime;
		float m_transitionDuration;
	};
}

#endif // ANIMATIONBLENDERHAVOK_INCLUDED
