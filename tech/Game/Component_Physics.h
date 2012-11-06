/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(COMPONENT_PHYSICS_INCLUDED)
#define COMPONENT_PHYSICS_INCLUDED

#include "Game/Component.h"
#include "Gfx/GfxMeshInstance.h"

namespace Teardrop
{
	class ZoneObject;
	class World;
	class Transform;
	struct FourCC;

	class PhysicsComponent
		: public Component
	{
	public:
		TD_CLASS(PhysicsComponent, Component);

		PhysicsComponent();
		~PhysicsComponent();

		virtual bool initialize(const char* asset, const FourCC& type);
		virtual bool destroy();
		virtual bool update(float deltaT);
		virtual bool addToWorld(World* pWorld);
		virtual bool removeFromWorld(World* pWorld);
		virtual bool getWorldTransform(Transform& xform);
		virtual bool setWorldTransform(const Transform& xform);

		void addedToZone(Zone* pZone);
		void removeFromZone(Zone* pZone);

		// for debug display
		virtual GfxMeshInstance& getDebugMesh();

	protected:
		GfxMeshInstance m_debugMeshInstance;
		bool m_bDebugMeshInitialized;

		virtual void initializeDebugMesh();
		virtual void releaseDebugMesh();
	};
}

#endif // COMPONENT_PHYSICS_INCLUDED
