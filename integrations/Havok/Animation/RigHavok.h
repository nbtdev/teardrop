/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
