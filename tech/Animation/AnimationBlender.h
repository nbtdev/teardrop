/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(ANIMATIONBLENDER_INCLUDED)
#define ANIMATIONBLENDER_INCLUDED

#include "Math/Matrix44.h"
#include "Util/_String.h"
#include "Memory/Memory.h"
#include <vector>

namespace Teardrop
{
    class Rig;
	class Matrix44;
	class Quaternion;
	class Vector4;
	class SkeletonInstance;
	class BlendGraph;
	class String;

	class AnimationBlender
	{
	public:
		//! normal c'tor (cannot fail)
		AnimationBlender();
		//! d'tor (cannot fail)
		virtual ~AnimationBlender();

        virtual bool initialize(Rig* rig, const BlendGraph& def);
		virtual bool destroy();
		virtual bool advance(float timeStep);
		virtual bool transitionToState(const String& stateName);

		//virtual bool addAnimation(HResource hAnim);
		virtual void setAdditionalRotation(size_t boneIndex, const Quaternion& rot);

		SkeletonInstance* getSkeletonInstance();

		struct MountPointData
		{
			Matrix44 m_transform;
			String m_name;
			size_t m_boneIndex;
		};	

		virtual size_t getNumMountPoints();
		// returns 0 if the mount point does not exist
		virtual const MountPointData* getMountPoint(size_t idx);

		TD_DECLARE_ALLOCATOR();

	protected:
        Rig* m_pRig;
		size_t m_instanceIdx;

	private:
		AnimationBlender(const AnimationBlender& other);
		AnimationBlender& operator=(const AnimationBlender& other);
	};
}

#endif // ANIMATIONBLENDER_INCLUDED
