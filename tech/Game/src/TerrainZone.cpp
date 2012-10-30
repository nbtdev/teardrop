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

#include "TerrainZone.h"
#include "TerrainPatch.h"
#include "Stream/include/Stream.h"
#include "ZoneObject.h"
#include "Octree/Octree.h"
#include "Scene.h"
#include "SceneUtil.h"
#include "Gfx/include/GfxCamera.h"
#include "Gfx/include/GfxBitmap.h"
#include "Gfx/include/GfxMaterial.h"
#include "Gfx/include/GfxTextureStage.h"
#include "Gfx/include/GfxTexture.h"
#include "Math/include/Transform.h"
#include "Math/include/MathUtil.h"
#include "Util/include/Environment.h"
#include "Resource/include/ResourceManager.h"
#include "Memory/include/Memory.h"
#include "Physics/include/Physics.h"
#include "Physics/include/Shape.h"
#include "Physics/include/Body.h"
#include "Physics/include/World.h"
#include "Reflection/include/PropertyDef.h"

using namespace CoS;
//---------------------------------------------------------------------------
COS_CLASS_IMPL(TerrainZone);
static const char* ZONE_TYPE = "terrain"; // TODO: put this in COS_CLASS?
//---------------------------------------------------------------------------
TerrainZone::TerrainZone()
{
	m_pTerrainMtl = 0;
	m_pHeightfieldBody = 0;
	m_pHeightfieldShape = 0;
}
//---------------------------------------------------------------------------
TerrainZone::~TerrainZone()
{
}
//---------------------------------------------------------------------------
const char* TerrainZone::getType() const
{
	return ZONE_TYPE;
}
//---------------------------------------------------------------------------
bool TerrainZone::initialize(World* pWorld)
{
	if(!Zone::initialize(pWorld))
	{
		return false;
	}

	if (!getHeightMap().length())
	{
		return false;
	}

	Vector4 size(getZoneSize());
	Vector2 tiling(getTiling());

	Environment& env = Environment::get();
	String hmap(m_dataRoot);
	hmap += "/";
	hmap += getHeightMap();

	// for now, just load the default test terrain
	GfxBitmap bmap;
	if (!bmap.load(hmap))
	{
		return false;
	}

	size_t chunksX = bmap.getWidth() / size_t(tiling.x);
	size_t chunksY = bmap.getHeight() / size_t(tiling.y);

	// create the single material shared by all terrain patches
	if (Environment::get().pRenderer)
	{
		String colorMap(m_dataRoot);
		colorMap += "/";
		colorMap += getDiffuseMap();
		String detailMap(m_dataRoot);
		detailMap += "/";
		detailMap += getDetailMap();

		m_pTerrainMtl = COS_NEW GfxMaterial;
		m_pTerrainMtl->initialize();
		m_pTerrainMtl->setDiffuse(0xFFFFFFFF); // white, for modulating against
		m_pTerrainMtl->setCustomShader(GfxMaterial::SHADER_TERRAIN);
		GfxTextureStage* pStage = m_pTerrainMtl->addTextureStage();
		pStage->setTexture(0, 
			env.pResourceMgr->acquire(GfxTexture::RESOURCE_TYPE, colorMap));
		pStage->setEnabled(true);
		pStage->setLayerBlendMode(GfxTextureStage::MODULATE);
		pStage->setFilter(GfxTextureStage::BILINEAR, GfxTextureStage::BILINEAR, GfxTextureStage::BILINEAR);
		pStage = m_pTerrainMtl->addTextureStage();
		pStage->setTexture(0, 
			env.pResourceMgr->acquire(GfxTexture::RESOURCE_TYPE, detailMap));
		pStage->setEnabled(true);
		pStage->setLayerBlendMode(GfxTextureStage::MODULATE);
		pStage->setFilter(GfxTextureStage::BILINEAR, GfxTextureStage::BILINEAR, GfxTextureStage::BILINEAR);

		// go through the heightfield and create terrain mesh patches
		size_t x = 0;
		size_t y = 0;
		for (y=0; y < chunksY; ++y)
		{
			for (x=0; x < chunksX; ++x)
			{
				// this gets owned and destroyed by the scene
				TerrainPatch* pPatch = COS_NEW TerrainPatch();
				pPatch->initialize();
				if (!pPatch->create(this, bmap, x, y))
				{
					delete pPatch;
					break;
				}
			}
		}
	}

	// create the heightfield in the collision world
	m_pHeightfieldShape = Physics::createHeightfieldShape(
		bmap.getData(),
		bmap.getWidth(),
		bmap.getHeight(),
		bmap.getDepth(),
		size);

	m_pHeightfieldBody = Physics::createBody();
	m_pHeightfieldBody->initialize(
		m_pHeightfieldShape, Body::STATIC, Vector4(-size.x/2, 0, -size.z/2, 0));
	m_pWorld->add(m_pHeightfieldBody);

	return true;
}
//---------------------------------------------------------------------------
bool TerrainZone::destroy()
{
	if (m_pHeightfieldBody && m_pWorld)
	{
		m_pWorld->remove(m_pHeightfieldBody);
		Physics::destroyBody(m_pHeightfieldBody);
		Physics::destroyShape(m_pHeightfieldShape);
	}

	if (m_pTerrainMtl)
	{
		m_pTerrainMtl->destroy();
		delete m_pTerrainMtl;
		m_pTerrainMtl = 0;
	}

	// since SpawnPoint is now just a ZoneObject like any other, it will be 
	// cleaned up with the rest of them; m_spawnPoints is just a cache of 
	// pointers so we can just empty it out
	m_spawnPoints.clear();

	// destroy/release the terrain tiles
	return Zone::destroy();
}
//---------------------------------------------------------------------------
bool TerrainZone::removeObject(ZoneObject* pObject)
{
	return Zone::removeObject(pObject);
}
//---------------------------------------------------------------------------
bool TerrainZone::addObject(ZoneObject* pObject)
{
	return Zone::addObject(pObject);
}
//---------------------------------------------------------------------------
void TerrainZone::addSpawnPoint(SpawnPoint* pSpawn)
{
	m_spawnPoints.push_back(pSpawn);
}
//---------------------------------------------------------------------------
size_t TerrainZone::getNumSpawnPoints() const
{
	return m_spawnPoints.size();
}
//---------------------------------------------------------------------------
void TerrainZone::freeSpawnPoint(size_t index)
{
	assert(index < m_spawnPoints.size());
	if (index < m_spawnPoints.size())
	{
		m_spawnPoints[index]->setValid(true);
	}
}
//---------------------------------------------------------------------------
const SpawnPoint** TerrainZone::getSpawnPoints() const
{
	return (const SpawnPoint**)&m_spawnPoints[0];
}
//---------------------------------------------------------------------------
const SpawnPoint& TerrainZone::getSpawnPoint(size_t index)
{
	//assert(index<m_spawnPoints.size());
	if (index >= m_spawnPoints.size())
		return INVALID_SPAWN;

	size_t idx = index;
	size_t nextSpawn = index;

	// if this one is taken, start down the list until we find a free one
	while(!m_spawnPoints[nextSpawn]->isValid())
	{
		nextSpawn = (++idx % m_spawnPoints.size());
		if (nextSpawn == index)
		{
			// we wrapped around, return invalid
			return INVALID_SPAWN;
		}

		if (!m_spawnPoints[nextSpawn]->isValid())
		{
			// we haven't wrapped, but this spawn point is already taken
			// or otherwise invalid, next one...
			continue;
		}
	}

	// we haven't wrapped to the start point, and this is valid,
	// so return it
	m_spawnPoints[nextSpawn]->setValid(false);
	m_spawnPoints[nextSpawn]->setSpawnIndex(nextSpawn);
	return *m_spawnPoints[nextSpawn];
}
//---------------------------------------------------------------------------
bool TerrainZone::getVisibleObjects(const Plane* frustum, ZoneObjects& objects)
{
	return Zone::getVisibleObjects(frustum, objects);
}
//---------------------------------------------------------------------------
GfxMaterial* TerrainZone::getTerrainMaterial()
{
	return m_pTerrainMtl;
}
//---------------------------------------------------------------------------
void TerrainZone::notifyPropertyChangedLocal(
	const Reflection::PropertyDef* pProp)
{
	if (String("Shader") == pProp->getName())
	{
	}
}
