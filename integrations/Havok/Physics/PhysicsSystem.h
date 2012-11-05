/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(HAVOK_PHYSICS_SYSTEM_H)
#define HAVOK_PHYSICS_SYSTEM_H

#include "Physics/Physics.h"

namespace CoS
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
				class System : public CoS::PhysicsSystem
				{
					CoS::Allocator* m_pAllocator;

				public:
					System();
					~System();

					// methods for managing the VDB
					void addWorldToDebugger(CoS::World* pWorld);
					void removeWorldFromDebugger(CoS::World* pWorld);
					void advanceDebugger(float deltaT);

					// Teardrop::System implementation
					void initialize();
					void shutdown();
					void getTypes(Type* typeArray, int& typeCount);
					void setAllocator(CoS::Allocator* pAllocator);
					CoS::Allocator* getAllocator();

					// CoS::PhysicsSystem implementation
					void destroyShape(CoS::Shape*);
					CoS::Shape* createHeightfieldShape(
						void* pData,
						size_t resX,
						size_t resZ,
						size_t bytesPerSample,
						const CoS::Vector4& scale);
					CoS::World* createWorld(const CoS::AABB& aabb, float cdTolerance=0.1f);
					CoS::Body* createBody();
					void destroyBody(CoS::Body*);
					CoS::Shape* createCapsule(
						const CoS::Vector4& p0,
						const CoS::Vector4& p1,
						float radius);
					void destroyCharacterProxy(CoS::CharacterProxy*);
					CoS::CharacterProxy* createCharacterProxy(
						CoS::Shape* pShape,
						const CoS::Vector4& worldPos,
						float mass,
						float maxSlopeInRadians,
						size_t materialId);
				};
			}
		}
	}
}

#endif // HAVOK_PHYSICS_SYSTEM_H
