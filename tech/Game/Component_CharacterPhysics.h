/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(COMPONENT_CHARACTERPHYSICS_INCLUDED)
#define COMPONENT_CHARACTERPHYSICS_INCLUDED

#include "Game/Component_Physics.h"
#include "Memory/Allocators.h"

namespace Teardrop
{
	class Vector4;
	class Transform;
	class CharacterProxy;
	class AnimationBlender;
	class RagdollController;
	class World;
	class Shape;

	class CharacterPhysicsComponent 
		: public PhysicsComponent
	{
		CharacterProxy* m_pProxy;
		RagdollController* m_pRagdoll;
		Shape* m_pShape;
		float m_gravVel; // gravity velocity contrib
		float m_accumTime;
		bool m_bRagdollNeedsAdded;
		World* m_pWorld; // cached only for the ragdoll!!!

	public:
		TD_CLASS(CharacterPhysicsComponent, PhysicsComponent);
		TD_CLASS_CREATABLE();

		TD_PROPERTY(RagdollName, "Optional ragdoll to use for character proxy", String, "", 0);

		CharacterPhysicsComponent();
		~CharacterPhysicsComponent();

		bool initialize();
		bool initialize(const char* asset, const FourCC& type);
		bool destroy();
		bool update(float deltaT);
		void driveRagdoll(float deltaT, AnimationBlender* pBlender, const Transform& world);
		bool addToWorld(World* pWorld);
		bool removeFromWorld(World* pWorld);

		void setLinearVelocity(const Vector4& vel);
		void setAngularVelocity(const Vector4& vel);
		bool setWorldTransform(const Transform& xform);
		bool getWorldTransform(Transform& xform);

		void addedToZone(Zone* pZone);
		void removeFromZone(Zone* pZone);

		bool isSupported() const;

		TD_DECLARE_ALLOCATOR();

	protected:
		void initializeDebugMesh();
	};
}

#endif // COMPONENT_CHARACTERPHYSICS_INCLUDED
