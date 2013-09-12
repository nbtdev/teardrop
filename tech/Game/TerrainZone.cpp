/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "TerrainZone.h"
#include "TerrainPatch.h"
#include "Stream/Stream.h"
#include "ZoneObject.h"
#include "Octree/Octree.h"
#include "Scene.h"
#include "SceneUtil.h"
#include "Gfx/GfxCamera.h"
#include "Gfx/GfxBitmap.h"
#include "Gfx/GfxMaterial.h"
#include "Gfx/GfxTextureStage.h"
#include "Gfx/GfxTexture.h"
#include "Math/Transform.h"
#include "Math/MathUtil.h"
#include "Util/Environment.h"
#include "Util/SystemManager.h"
#include "Resource/ResourceManager.h"
#include "Memory/Memory.h"
#include "Physics/Physics.h"
#include "Physics/Shape.h"
#include "Physics/Body.h"
#include "Physics/World.h"
#include "Reflection/PropertyDef.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
TD_CLASS_IMPL(TerrainZone);
static const char* ZONE_TYPE = "terrain"; // TODO: put this in TD_CLASS?
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

		m_pTerrainMtl = TD_NEW GfxMaterial;
		m_pTerrainMtl->initialize();
		//m_pTerrainMtl->setDiffuse(0xFFFFFFFF); // white, for modulating against
		m_pTerrainMtl->setCustomShader(GfxMaterial::SHADER_TERRAIN);
		GfxTextureStage* pStage = m_pTerrainMtl->addTextureStage();
		pStage->setTexture(0, 
			env.pResourceMgr->acquire(GfxTexture::RESOURCE_TYPE, colorMap));
		pStage->setEnabled(true);
		pStage->setBlendMode(GfxTextureStage::BLENDMODE_MODULATE);
		pStage->setMinFilter(GfxTextureStage::FILTER_BILINEAR);
		pStage->setMagFilter(GfxTextureStage::FILTER_BILINEAR);
		pStage->setMipMapFilter(GfxTextureStage::FILTER_BILINEAR);
		pStage = m_pTerrainMtl->addTextureStage();
		pStage->setTexture(0, 
			env.pResourceMgr->acquire(GfxTexture::RESOURCE_TYPE, detailMap));
		pStage->setEnabled(true);
		pStage->setBlendMode(GfxTextureStage::BLENDMODE_MODULATE);
		pStage->setMinFilter(GfxTextureStage::FILTER_BILINEAR);
		pStage->setMagFilter(GfxTextureStage::FILTER_BILINEAR);
		pStage->setMipMapFilter(GfxTextureStage::FILTER_BILINEAR);

		// go through the heightfield and create terrain mesh patches
		size_t x = 0;
		size_t y = 0;
		for (y=0; y < chunksY; ++y)
		{
			for (x=0; x < chunksX; ++x)
			{
				// this gets owned and destroyed by the scene
				TerrainPatch* pPatch = TD_NEW TerrainPatch();
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
	PhysicsSystem* pSys = static_cast<PhysicsSystem*>(
		Environment::get().pSystemMgr->getActiveSystem(System::SYSTEM_PHYSICS));
	m_pHeightfieldShape = pSys->createHeightfieldShape(
		bmap.getData(),
		bmap.getWidth(),
		bmap.getHeight(),
		bmap.getDepth(),
		size);

	m_pHeightfieldBody = pSys->createBody();
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

		PhysicsSystem* pSys = static_cast<PhysicsSystem*>(
			Environment::get().pSystemMgr->getActiveSystem(System::SYSTEM_PHYSICS));
		pSys->destroyBody(m_pHeightfieldBody);
		pSys->destroyShape(m_pHeightfieldShape);
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
