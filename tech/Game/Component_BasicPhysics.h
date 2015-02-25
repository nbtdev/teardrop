/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(COMPONENT_BASICPHYSICS_INCLUDED)
#define COMPONENT_BASICPHYSICS_INCLUDED

#include "Game/Component_Physics.h"
#include "Memory/Allocators.h"

namespace Teardrop
{
	class Body;
	class World;

	class BasicPhysicsComponent 
		: public PhysicsComponent
	{
        Body* m_pBody;
		World* m_pWorld;

		enum Operation
		{
			NONE = 0,
			ADD,
			REMOVE,
		};
		Operation m_pendingOp;

	public:
		TD_CLASS(BasicPhysicsComponent, PhysicsComponent);
		TD_CLASS_CREATABLE();

		TD_PROPERTY(RigidBodyName, "Name of rigid body asset file to use", String, "(undefined)", FileChooser);

		BasicPhysicsComponent();
		virtual ~BasicPhysicsComponent();

		bool initialize();
		bool destroy();
		bool update(float deltaT);
		bool addToWorld(World* pWorld);
		bool removeFromWorld(World* pWorld);
		bool getWorldTransform(Transform& xform);
		bool setWorldTransform(const Transform& xform);
		bool setBody(const char* assetFilename);

		void addedToZone(Zone* pZone);
		void removeFromZone(Zone* pZone);

		TD_DECLARE_ALLOCATOR();

	protected:
		void initializeDebugMesh();
	};
}

#endif // COMPONENT_BASICPHYSICS_INCLUDED
