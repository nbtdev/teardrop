/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TERRAINZONE_INCLUDED)
#define TERRAINZONE_INCLUDED

#include "Game/Zone.h"
#include "Game/SpawnPoint.h"
#include "Memory/Allocators.h"
#include "Math/Vector2.h"

namespace Teardrop
{
	class GfxMaterial;
	class Body;
	class Shape;

	class TerrainZone 
		: public Zone
	{
		GfxMaterial* m_pTerrainMtl;
		Body* m_pHeightfieldBody;
		Shape* m_pHeightfieldShape;

		typedef std::vector<SpawnPoint*> SpawnPoints;
		SpawnPoints m_spawnPoints;

	public:
		TD_CLASS(TerrainZone, Zone);
		TD_CLASS_CREATABLE();

		TD_PROPERTY(HeightMap, "Name of file containing the heightmap (8-bit grayscale)", String, "(undefined)", 0);
		TD_PROPERTY(DiffuseMap, "Name of file containing the diffuse texture", String, "(undefined)", 0);
		TD_PROPERTY(DetailMap, "Name of file containing the detailtexture", String, "(undefined)", 0);
		TD_PROPERTY(Shader, "Name of 3D terrain shader source file", String, "", 0);
		TD_PROPERTY(CastShadows, "Whether terrain casts shadows (default: yes)", bool, true, 0);
		TD_PROPERTY(ReceiveShadows, "Whether terrain receives shadows (default: yes)", bool, true, 0);
		TD_COMPLEX_PROPERTY(ZoneSize, "Size of zone width, height and depth", Vector4, "(0,0,0,0)", 0);
		TD_COMPLEX_PROPERTY(Tiling, "Number of terrain tiles in X and Z ", Vector2, "(0,0)", 0);

		TerrainZone();
		~TerrainZone();

		bool initialize(World* pWorld);
		bool destroy();
		const char* getType() const;

		bool addObject(ZoneObject* pObject);
		bool removeObject(ZoneObject* pObject);
		bool getVisibleObjects(const Plane* frustum, ZoneObjects& objects);
		GfxMaterial* getTerrainMaterial();

		void addSpawnPoint(SpawnPoint* pSpawn);
		size_t getNumSpawnPoints() const;
		const SpawnPoint& getSpawnPoint(size_t index);
		void freeSpawnPoint(size_t index);
		const SpawnPoint** getSpawnPoints() const;

		void notifyPropertyChangedLocal(const Reflection::PropertyDef*);

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // TERRAINZONE_INCLUDED
