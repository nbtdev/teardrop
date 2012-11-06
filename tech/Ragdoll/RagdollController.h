/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RAGDOLLCONTROLLER_INCLUDED)
#define RAGDOLLCONTROLLER_INCLUDED

#include "Resource/ResourceHandle.h"
#include "Memory/Memory.h"

#include "Math/Vector4.h"
#include <vector>
#include <list>

namespace Teardrop
{
	class AnimationBlender;
	class Transform;
	class World;

	class RagdollController
	{
	public:
		//! normal c'tor (cannot fail)
		RagdollController();
		//! d'tor (cannot fail)
		virtual ~RagdollController();

		virtual bool initialize(HResource hRagdoll);
		virtual bool destroy();
		virtual bool drive(float timeStep, AnimationBlender* pBlender, const Transform& world);
		virtual bool addToWorld(World* pWorld);
		virtual bool removeFromWorld(World* pWorld);

		// debug view of the ragdoll's shapes
		typedef std::vector<Vector4> DisplayGeometry;
		typedef std::list<DisplayGeometry> DisplayGeometries;
		// will return a list of lines (start end start end ...) that describe 
		// the debug view of the ragdoll's geometries
		virtual bool getDisplayGeometry(DisplayGeometries& geom) = 0;

		virtual void setUserData(void*);

		TD_DECLARE_ALLOCATOR();

	protected:
		HResource m_hRagdoll;

	private:
		RagdollController(const RagdollController& other);
		RagdollController& operator=(const RagdollController& other);
	};
}

#endif // RAGDOLLCONTROLLER_INCLUDED
