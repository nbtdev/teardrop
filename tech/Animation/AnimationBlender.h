/******************************************************************************
Copyright (c) 2018 Teardrop Games

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
******************************************************************************/

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
