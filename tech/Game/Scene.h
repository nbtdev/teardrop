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

#if !defined(SCENE_INCLUDED)
#define SCENE_INCLUDED

#include <vector>
#include "Memory/Allocators.h"

namespace CoS
{
	class Zone;
	class ZoneObject;
	class ZoneObjects;
	class Stream;
	class String;
	class Plane;
	struct FourCC;
	class World;
	class LoadListener;
	class Vector4;
	class Ray;

	namespace Reflection
	{
		class ClassDef;
	}

	class Scene
	{
		friend struct SceneUtil;

		typedef std::vector<size_t> ZoneNeighbors;
		typedef struct ZoneNode
		{
			Zone* pZone;
			ZoneNeighbors neighbors;

			bool operator==(const ZoneNode& other)
			{
				return (pZone == other.pZone);
			}
		} ZoneNode;

		typedef std::vector<ZoneNode> Zones;

		size_t m_currentZone;
		Zones m_zones;
		World* m_pWorld;
		float m_accumTime;

	public:
		static const FourCC& RESOURCE_TYPE;

		Scene();
		~Scene();

		bool initialize();
		bool destroy();
		bool update(float deltaT);

		Zone* createZone(const char* type, LoadListener* pCallback=0);
		void destroyZone(Zone* pZone);
		bool addNeighbor(const Zone* pMe, const Zone* pNeighbor);
		bool getVisibleObjects(const Plane* frustum, ZoneObjects& objects) const;
		const Vector4& getAmbientLight() const;
		void setupCollisionFilters();

		/*
			find object(s) intersecting with "ray"
		*/
		bool getIntersectingObjects(
			/*in*/ const Ray& ray, 
			/*out*/ZoneObjects& objects, 
			bool sort=false,	// will sort hits in order of increasing distance from camera
			bool precise=false	// will use precise (polygon-level) raycasting for more accuracy
			) const;
		bool getIntersectionPoints(/*in*/const Ray& ray, /*out*/Vector4* pPoints, /*inout*/size_t& pointCount);

		// scene query methods
		typedef bool (*FilterFunc)(ZoneObject*);
		bool findObjectsWithinRadius(
			ZoneObjects& objects, 
			const Vector4& origin,
			float radius, 
			FilterFunc fn=0) const;

		/** Accessors
		*/
		Zone* getCurrentZone() const;

		/** Mutators
		*/
		bool setCurrentZone(const Zone& zone);

		COS_DECLARE_ALLOCATOR();

		World* getWorld();

		// find all objects in the scene of a given type
		void findAllObjectsOfType(
			/*in*/const Reflection::ClassDef* pClass, 
			/*out*/ZoneObjects& list);

	private:
	};
}

#endif // SCENE_INCLUDED
