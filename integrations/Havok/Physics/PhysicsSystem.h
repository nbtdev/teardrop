/******************************************************************************
Copyright (c) 2015 Teardrop Games

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
******************************************************************************/

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
