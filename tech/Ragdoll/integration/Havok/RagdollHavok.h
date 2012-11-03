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

#if !defined(RAGDOLLHAVOK_INCLUDED)
#define RAGDOLLHAVOK_INCLUDED

#include "Memory/include/Allocators.h"
#include "Ragdoll/include/Ragdoll.h"

class hkpPhysicsSystem;
class hkResource;
class hkaSkeletonMapper;
class hkaRagdollInstance;

namespace CoS
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

		COS_DECLARE_ALLOCATOR();

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
