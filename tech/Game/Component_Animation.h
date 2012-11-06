/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(COMPONENT_ANIMATION_INCLUDED)
#define COMPONENT_ANIMATION_INCLUDED

#include "Resource/ResourceHandle.h"
#include <map>
#include "Game/Component.h"
#include "Memory/Allocators.h"

namespace Teardrop
{
	struct Environment;
	struct FourCC;
	class ZoneObject;
	class AnimationBlender;
	class SkeletonInstance;
	class Matrix44;
	class Quaternion;

	class AnimationComponent
		: public Component
	{
		HResource m_hRig;
		typedef std::map<String, HResource> AnimationLUT;
		AnimationLUT m_lut;
		AnimationBlender* m_pBlender;

	public:
		TD_CLASS(AnimationComponent, Component);
		TD_CLASS_CREATABLE();

		TD_PROPERTY(RigName, "Name of rig (skeleton) asset file to use", String, "(undefined)", FileChooser);
		TD_PROPERTY(BlendConfigName, "Names of animation blend tree config file to use", String, "", 0);

		static const String& SHORT_NAME;
		AnimationComponent();
		virtual ~AnimationComponent();

		bool initialize();
		bool destroy();
		bool addAnimation(const char* name, const char* assetPath);
		bool addRig(const char* assetPath);
		bool update(float deltaT);
		const Matrix44* getBoneCache() const;
		size_t getNumBones() const;
		AnimationBlender* getBlender();

		size_t getBoneIndexByName(const String& name);
		void setAdditionalBoneRotationByIndex(size_t idx, const Quaternion& rot);
		void transitionToState(const String& state);

		TD_DECLARE_ALLOCATOR();

	protected:
	};
}

#endif // COMPONENT_ANIMATION_INCLUDED
