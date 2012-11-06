/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(ZONE_INCLUDED)
#define ZONE_INCLUDED

#include <vector>
#include <list>
#include "Game/ZoneObjects.h"
#include "Math/AABB.h"
#include "Memory/Allocators.h"
#include "Reflection/Reflection.h"

class TiXmlElement;

namespace Teardrop
{
	class ZoneObject;
	class Stream;
	class Plane;
	class SpatialPartitioning;
	class World;
	class Scene;
	class LoadListener;
	class Ray;
	class SpawnPoint;
	class DynamicLight;

	class Zone
		: public Reflection::Object
	{
		friend class Scene;

		Scene* m_pScene;
		LoadListener* m_pLoadListener;
		char m_name[64];

		typedef std::vector<DynamicLight*> ShadowCastingLights;
		ShadowCastingLights m_shadowCastingLights;

	public:
		static const size_t INVALID = 0xFFFFFFFF;
		static const SpawnPoint& INVALID_SPAWN;

		TD_CLASS(Zone, Object);
		TD_PROPERTY(Name, "Zone Name", String, "(undefined)", 0);
		TD_COMPLEX_PROPERTY(Ambient, "Zone's Ambient Light Color", Vector4, "(0,0,0,0)", 0);
		TD_COMPLEX_PROPERTY(Dimensions, "Zone's width, height and depth", Vector4, "(0,0,0,0)", 0);
		TD_COMPLEX_PROPERTY(Origin, "Zone's World Origin", Vector4, "(0,0,0,0)", 0);

		Zone();
		virtual ~Zone();

		virtual bool initialize(World* pWorld);
		virtual bool destroy();
		virtual const char* getType() const;
		const ZoneObjects& getZoneObjects() const;

		virtual bool addObject(ZoneObject* pObject);
		virtual bool removeObject(ZoneObject* pObject);
		virtual void objectMoved(ZoneObject* pObject);
		virtual bool getVisibleObjects(const Plane* frustum, ZoneObjects& objects) const;

		void update(float deltaT); // called by the parent scene to update dynamic objects

		/*
			"sort" will sort hits in order of increasing distance from camera
		*/
		bool getIntersectingObjects(const Ray& ray, /*out*/ZoneObjects& objects, bool sort=false) const;

		typedef bool (*FilterFunc)(ZoneObject*);
		bool getObjectsWithinRadius(
			const Vector4& origin, 
			float radius, 
			/*out*/ZoneObjects& objects,
			FilterFunc fn) const;

		// add a shadowcasting light to the cache of shadow-casting lights in this zone
		void addShadowCastingLight(DynamicLight* pLight);
		// remove a shadowcasting light from the cache of shadow-casting lights in this zone
		void removeShadowCastingLight(DynamicLight* pLight);
		// access shadowcasting lights
		size_t getNumShadowCastingLights();
		DynamicLight* getShadowCastingLight(size_t idx);

		/** Accessors
		*/
		const AABB& getAABB() const;
		size_t getId() const;
		Scene* getScene() const;

		// default implementations of these return 0 and INVALID_SPAWN
		virtual void addSpawnPoint(SpawnPoint* pSpawn);
		virtual size_t getNumSpawnPoints() const;
		virtual const SpawnPoint& getSpawnPoint(size_t index);
		virtual void freeSpawnPoint(size_t index);
		virtual const SpawnPoint** getSpawnPoints() const;
		const String& getDataRootPath() const { return m_dataRoot; }

		/** Mutators
		*/
		void setWorldPosition(const Vector4& pos);
		void setZoneDimensions(const Vector4& dimensions);
		void setId(size_t id);
		void setDataRootPath(const String& path);

		String toString();

		TD_DECLARE_ALLOCATOR();

	protected:
		ZoneObjects m_objects;
		
		// dynamic objects need updating to ensure they are fully initialized
		typedef std::list<unsigned int> DynamicObjects;
		DynamicObjects m_dynamicObjects;

		SpatialPartitioning* m_pSpace;
		AABB m_aabb;
		size_t m_id;
		World* m_pWorld;
		String m_dataRoot;
	};
}

#endif // ZONE_INCLUDED
