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

#if !defined(PHYSICS_INCLUDED)
#define PHYSICS_INCLUDED

namespace CoS
{
	class World;
	class Shape;
	class Body;
	class Vector4;
	class CharacterProxy;
	class AABB;

	class Physics
	{
		static Allocator* m_pAllocator;

	public:
		//! override memory allocator
		static Allocator* getAllocator();
		static void setAllocator(Allocator* pAllocator);

		//! initialize the physics/collision simulation engine
		static bool initialize(bool bEnableLogging = true, bool bEnableVDB = true);

		//! shutdown the physics/collision simulation engine
		static bool shutdown();

		//! add a physics world to the debugger
		static void addWorldToDebugger(World* pWorld);

		//! remove a world from the debugger
		static void removeWorldFromDebugger(World* pWorld);

		//! advance the debugger
		static void advanceDebugger(float deltaT);

		static void destroyShape(Shape*);

		/* 
			"factory" methods -- these call initialize() on the
			things they create, you get rid of them by calling destroy()
			and then delete on them (you own the object this method
			returns, in other words)
		*/
		static Shape* createHeightfieldShape(
			void* pData,
			size_t resX,
			size_t resZ,
			size_t bytesPerSample,
			const Vector4& scale);

		//! aabb is the world aabb
		//! cdTolerance is the distance between objects before they are considered colliding
		static World* createWorld(const AABB& aabb, float cdTolerance=0.1f);

		//! create/delete a body
		static Body* createBody();
		static void destroyBody(Body*);

		//! create a capsule, oriented along the axis provided
		static Shape* createCapsule(
			const Vector4& p0,
			const Vector4& p1,
			float radius);

		//! create a character proxy, using the supplied shape
		static void destroyCharacterProxy(CharacterProxy*);
		static CharacterProxy* createCharacterProxy(
			Shape* pShape,
			const Vector4& worldPos,
			float mass,
			float maxSlopeInRadians,
			size_t materialId);
	};
}

#endif // PHYSICS_INCLUDED
