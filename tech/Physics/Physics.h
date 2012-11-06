/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PHYSICS_SYSTEM_INCLUDED)
#define PHYSICS_SYSTEM_INCLUDED

#include "Util/System.h"

namespace Teardrop
{
	class World;
	class Shape;
	class Body;
	class Vector4;
	class CharacterProxy;
	class Allocator;
	class AABB;

	class PhysicsSystem : public Teardrop::System
	{
	public:
		//! destroy any previously-created shape instance
		virtual void destroyShape(Shape*) = 0;

		/* 
			"factory" methods -- these call initialize() on the
			things they create, you get rid of them by calling destroy()
			and then delete on them (you own the object this method
			returns, in other words)
		*/
		virtual Shape* createHeightfieldShape(
			void* pData,
			size_t resX,
			size_t resZ,
			size_t bytesPerSample,
			const Vector4& scale) = 0;

		//! aabb is the world aabb
		//! cdTolerance is the distance between objects before they are considered colliding
		virtual World* createWorld(const AABB& aabb, float cdTolerance=0.1f) = 0;

		//! create/delete a body
		virtual Body* createBody() = 0;
		virtual void destroyBody(Body*) = 0;

		//! create a capsule, oriented along the axis provided
		virtual Shape* createCapsule(
			const Vector4& p0,
			const Vector4& p1,
			float radius) = 0;

		//! destroy a previously-created character proxy
		virtual void destroyCharacterProxy(CharacterProxy*) = 0;
		//! create a character proxy, using the supplied shape
		virtual CharacterProxy* createCharacterProxy(
			Shape* pShape,
			const Vector4& worldPos,
			float mass,
			float maxSlopeInRadians,
			size_t materialId) = 0;
	};
}

#endif // PHYSICS_SYSTEM_INCLUDED
