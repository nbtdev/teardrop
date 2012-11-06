/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(HAVOK_PHYSICS_SYSTEM_H)
#define HAVOK_PHYSICS_SYSTEM_H

#include "Physics/Physics.h"

namespace Teardrop
{
	class Allocator;
}

namespace Teardrop
{
	namespace Integration
	{
		namespace Havok
		{
			namespace Physics
			{
				class System : public Teardrop::PhysicsSystem
				{
					Teardrop::Allocator* m_pAllocator;

				public:
					System();
					~System();

					// methods for managing the VDB
					void addWorldToDebugger(Teardrop::World* pWorld);
					void removeWorldFromDebugger(Teardrop::World* pWorld);
					void advanceDebugger(float deltaT);

					// Teardrop::System implementation
					void initialize();
					void shutdown();
					void getTypes(Type* typeArray, int& typeCount);
					void setAllocator(Teardrop::Allocator* pAllocator);
					Teardrop::Allocator* getAllocator();

					// Teardrop::PhysicsSystem implementation
					void destroyShape(Teardrop::Shape*);
					Teardrop::Shape* createHeightfieldShape(
						void* pData,
						size_t resX,
						size_t resZ,
						size_t bytesPerSample,
						const Teardrop::Vector4& scale);
					Teardrop::World* createWorld(const Teardrop::AABB& aabb, float cdTolerance=0.1f);
					Teardrop::Body* createBody();
					void destroyBody(Teardrop::Body*);
					Teardrop::Shape* createCapsule(
						const Teardrop::Vector4& p0,
						const Teardrop::Vector4& p1,
						float radius);
					void destroyCharacterProxy(Teardrop::CharacterProxy*);
					Teardrop::CharacterProxy* createCharacterProxy(
						Teardrop::Shape* pShape,
						const Teardrop::Vector4& worldPos,
						float mass,
						float maxSlopeInRadians,
						size_t materialId);
				};
			}
		}
	}
}

#endif // HAVOK_PHYSICS_SYSTEM_H
