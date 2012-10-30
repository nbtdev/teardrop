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

#if !defined(COMPONENT_CHARACTERPHYSICS_INCLUDED)
#define COMPONENT_CHARACTERPHYSICS_INCLUDED

#include "Game/include/Component_Physics.h"
#include "Memory/include/Allocators.h"

namespace CoS
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
		COS_CLASS(CharacterPhysicsComponent, PhysicsComponent);
		COS_CLASS_CREATABLE();

		COS_PROPERTY(RagdollName, "Optional ragdoll to use for character proxy", String, "", 0);

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

		COS_DECLARE_ALLOCATOR();

	protected:
		void initializeDebugMesh();
	};
}

#endif // COMPONENT_CHARACTERPHYSICS_INCLUDED
