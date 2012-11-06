/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RAGDOLLCONTROLLERHAVOK_INCLUDED)
#define RAGDOLLCONTROLLERHAVOK_INCLUDED

#include "Ragdoll/RagdollController.h"

class hkaRagdollRigidBodyController;

namespace Teardrop
{
	class RagdollControllerHavok
		: public RagdollController
	{
	public:
		//! normal c'tor (cannot fail)
		RagdollControllerHavok();
		//! d'tor (cannot fail)
		~RagdollControllerHavok();

		bool initialize(HResource hRagdoll);
		bool destroy();
		bool drive(float timeStep, AnimationBlender* pBlender, const Transform& world);
		bool addToWorld(World* pWorld);
		bool removeFromWorld(World* pWorld);
		void setUserData(void*);

		bool getDisplayGeometry(DisplayGeometries& geom);

		TD_DECLARE_ALLOCATOR();

	private:
		hkaRagdollRigidBodyController* m_pController;
		hkaRagdollInstance* m_pInstance;
		void *m_pUD;

		RagdollControllerHavok(const RagdollControllerHavok& other);
		RagdollControllerHavok& operator=(const RagdollControllerHavok& other);
	};
}

#endif // RAGDOLLCONTROLLERHAVOK_INCLUDED
