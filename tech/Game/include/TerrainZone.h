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

#if !defined(TERRAINZONE_INCLUDED)
#define TERRAINZONE_INCLUDED

#include "Game/include/Zone.h"
#include "Game/include/SpawnPoint.h"
#include "Memory/include/Allocators.h"
#include "Math/include/Vector2.h"

namespace CoS
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
		COS_CLASS(TerrainZone, Zone);
		COS_CLASS_CREATABLE();

		COS_PROPERTY(HeightMap, "Name of file containing the heightmap (8-bit grayscale)", String, "(undefined)", 0);
		COS_PROPERTY(DiffuseMap, "Name of file containing the diffuse texture", String, "(undefined)", 0);
		COS_PROPERTY(DetailMap, "Name of file containing the detailtexture", String, "(undefined)", 0);
		COS_PROPERTY(Shader, "Name of 3D terrain shader source file", String, "", 0);
		COS_PROPERTY(CastShadows, "Whether terrain casts shadows (default: yes)", bool, true, 0);
		COS_PROPERTY(ReceiveShadows, "Whether terrain receives shadows (default: yes)", bool, true, 0);
		COS_COMPLEX_PROPERTY(ZoneSize, "Size of zone width, height and depth", Vector4, "(0,0,0,0)", 0);
		COS_COMPLEX_PROPERTY(Tiling, "Number of terrain tiles in X and Z ", Vector2, "(0,0)", 0);

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

		COS_DECLARE_ALLOCATOR();
	};
}

#endif // TERRAINZONE_INCLUDED
