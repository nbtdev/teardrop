/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RAGDOLLHAVOK_INCLUDED)
#define RAGDOLLHAVOK_INCLUDED

#include "Memory/Allocators.h"
#include "Ragdoll/Ragdoll.h"

class hkpPhysicsSystem;
class hkResource;
class hkaSkeletonMapper;
class hkaRagdollInstance;

namespace Teardrop
{
	class RagdollHavok : public Ragdoll
	{
		DECLARE_SERIALIZABLE(RagdollHavok);

	public:
		//! normal c'tor (cannot fail)
		RagdollHavok();
		//! placement c'tor (cannot fail)
		RagdollHavok(int);
		//! d'tor (cannot fail)
		~RagdollHavok();

		bool initialize(
			void* pData,
			unsigned int dataLen
			);

		bool initialize();

		bool release();
		bool update(float deltaT, AnimationBlender* pAnimBlender);

		Ragdoll* clone();
		// if you need the rigid body hierarchy (hkArray) use this method
		// to access the hkpPhysicsSystem, and then the returned pointer (if
		// valid) to access the hkArray of rigid bodies
		hkpPhysicsSystem* getPhysicsSystem();
		// use this to get the high-to-low (anim-to-ragdoll) mapper that was 
		// exported with the radgoll container
		hkaSkeletonMapper* getHighToLowSkeletonMapper();
		// use this to get the low-to-high (ragdoll-to-anim) mapper that was 
		// exported with the radgoll container
		hkaSkeletonMapper* getLowToHighSkeletonMapper();
		// use this to get the ragdoll instance that was stored in the
		// ragdoll container; do not alter this guy!!!
		hkaRagdollInstance* getRagdollInstance();
		// use this to get a CLONE of the ragdoll instance that was stored in the
		// ragdoll container; you are free to alter this guy all you want...
		hkaRagdollInstance* cloneRagdollInstance();

		/**
			Serialization
		*/
		//! package for storage
		bool serialize(ResourceSerializer& serializer);

		TD_DECLARE_ALLOCATOR();

	protected:
		SerialPointer<hkResource> m_pResource;
		// we use the hkpPhysicsSystem because it is unnecessary and
		// unwieldy to carry around an hkArray<hkpRigidBody*> in this member;
		// the primary reason we want a pointer here anyway is to identify 
		// when a ragdoll definition is loaded and valid (initialized); we 
		// can always get the hkArray<hkpRigidBody> from the physics system
		SerialPointer<hkpPhysicsSystem> m_pRagdollBodies;
		// cached pointers
		SerialPointer<hkaSkeletonMapper> m_pHighToLowMapper;
		SerialPointer<hkaSkeletonMapper> m_pLowToHighMapper;
		SerialPointer<hkaRagdollInstance> m_pInstance;

	private:
		void delayInit();
	};
}

#endif // RAGDOLLHAVOK_INCLUDED
