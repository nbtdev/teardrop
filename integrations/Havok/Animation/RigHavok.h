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

#if !defined(RIGHAVOK_INCLUDED)
#define RIGHAVOK_INCLUDED

#include "Animation/Rig.h"
#include "Serialization/SerialPointerArray.h"

class hkaSkeleton;
class hkaAnimatedSkeleton;
class hkResource;

namespace Teardrop
{
	class BoneHavok;
	class SkeletonInstanceHavok;

	class RigHavok : public Rig
	{
		DECLARE_SERIALIZABLE(RigHavok);

		SerialPointer<hkaSkeleton> m_pRig;
		SerialPointer<BoneHavok> m_pBones;
		SerialPointer<hkResource> m_pResource;
		SerialPointerArray<SkeletonInstanceHavok> m_instances;

		void delayInit();

	public:
		RigHavok();
		~RigHavok();

		bool initialize(void* pData, unsigned int dataLen);
		bool destroy();
		bool release();

		Bone* getBone(const char* name);
		Bone* getBone(size_t index);
		size_t getBoneCount();
		
		size_t createInstance();
		SkeletonInstance* getInstance(size_t idx);
		hkaSkeleton* getSkeleton();

		/**
			Serialization
		*/
		//! package for storage
		bool serialize(ResourceSerializer& serializer);

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // RIGHAVOK_INCLUDED
