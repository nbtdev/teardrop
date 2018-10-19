/******************************************************************************
Copyright (c) 2018 Teardrop Games

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

#include "TerrainPatch.h"
#include "TerrainZone.h"
#include "Component_Render.h"
//#include "Gfx/Bitmap.h"
//#include "Gfx/VertexFormat.h"
#include "Gfx/Renderer.h"
//#include "Gfx/Texture.h"
//#include "Gfx/TextureStage.h"
//#include "Gfx/Util.h"
#include "Gfx/Mesh.h"
//#include "Gfx/SubMesh.h"
#include "Gfx/Material.h"
//#include "Gfx/MeshInstance.h"
//#include "Gfx/VertexData.h"
//#include "Gfx/IndexData.h"
#include "Gfx/ShaderConstantTable.h"
#include "Gfx/ShaderConstant.h"
#include "Util/Environment.h"
#include "Util/Hash.h"
#include "Math/Transform.h"
#include "Math/MathUtil.h"
#include "Memory/Memory.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
TD_CLASS_IMPL(TerrainPatch);
//---------------------------------------------------------------------------
struct Teardrop::TerrainVertex
{
	float x, y, z;
	//float u1, v1; // diffuse map
	//float u2, v2; // detail map

	TD_DECLARE_ALLOCATOR();
};
//! normals used only for lit terrain
struct Teardrop::TerrainNormal
{
	float nx, ny, nz;

	TD_DECLARE_ALLOCATOR();
};
//---------------------------------------------------------------------------
TerrainPatch::TerrainPatch()
{
	m_pVertexBuffer = 0;
	m_pNormalBuffer = 0;
	m_pIndexBuffer = 0;
}
//---------------------------------------------------------------------------
TerrainPatch::~TerrainPatch()
{
	// TODO: we can't really do this because they back graphics resources,
	// and we should wait until the graphics objects are actually released, 
	// but for now it'll have to do
	delete [] m_pVertexBuffer;
	delete [] m_pNormalBuffer;
	GetDEFAULTAllocator()->DeallocateAligned(m_pIndexBuffer);
}
//---------------------------------------------------------------------------
bool TerrainPatch::initialize()
{
	ZoneObject::initialize();
	return true;
}
//---------------------------------------------------------------------------
bool TerrainPatch::destroy()
{
    //Environment::get().pResourceMgr->release(m_hMesh);
	ZoneObject::destroy();
	return true;
}
//---------------------------------------------------------------------------
#if 0
bool TerrainPatch::create(
	TerrainZone* /*pOwner*/, 
    const Gfx::Bitmap& /*heightmap*/,
	size_t /*x*/, 
	size_t /*y*/)
{
	m_pOwner = pOwner;

	float xStep, zStep; // one-over the width and height of the patch
	float uStart, vStart; // starting U and V coordinates for the patch 
	float uStep, vStep; // one-over the width and height of the terrain
	float xScale, zScale; // world size of a patch

	GfxMeshInstance inst;
	m_hMesh = Environment::get().pResourceMgr->acquire(GfxMesh::RESOURCE_TYPE);
	inst.setMeshHandle(m_hMesh);
//	inst.setRenderCallback(this);

	size_t tileWidth = size_t(pOwner->getTiling().x);
	size_t tileHeight = size_t(pOwner->getTiling().y);
	size_t patchWidth = tileWidth + 1;
	size_t patchHeight = tileHeight + 1;
	size_t patchesX = heightmap.getWidth() / tileWidth;
	size_t patchesY = heightmap.getHeight() / tileHeight;

	xScale = pOwner->getZoneSize().x / (float)patchesX;
	zScale = pOwner->getZoneSize().z / (float)patchesY;
	
	// if the index is beyond the number of patches indicated by the patch
	// dimensions specified in the data, return false
	if ((x * y) >= (patchesX * patchesY))
	{
		return false;
	}

	xStep = 1.0f / (float)tileWidth;
	float oo255 = 1.0f / 255.f;
	zStep = 1.0f / (float)tileHeight;
	char* pData = (char*)heightmap.getData();
	size_t imgWidth = heightmap.getWidth();
	size_t imgHeight = heightmap.getHeight();
	uStep = 1.f / (float)imgWidth;
	vStep = 1.f / (float)imgHeight;
	size_t xOff = x * tileWidth;
	size_t yOff = y * tileHeight;
	uStart = (float)xOff / (float)heightmap.getWidth();
	vStart = (float)yOff / (float)heightmap.getHeight();

	GfxMaterial* pMtl = m_pOwner->getTerrainMaterial();

	// create the vertex data buffer and format
	size_t numVerts = patchWidth * patchHeight;
	m_pVertexBuffer = TD_NEW TerrainVertex[numVerts];
	memset(m_pVertexBuffer, 0, sizeof(TerrainVertex) * numVerts);
	m_pNormalBuffer = TD_NEW TerrainNormal[numVerts];
	memset(m_pNormalBuffer, 0, sizeof(TerrainNormal) * numVerts);

	GfxVertexFormat vertFmt;
	GfxVertexFormat::Element elem;
	elem.offset = 0;
	elem.semantic = POSITION;
	elem.type = FLOAT3;
	elem.setUsage(0);
	vertFmt.addElement(elem);

	elem.offset = 0;
	elem.semantic = NORMAL;
	elem.type = FLOAT3;
	elem.setUsage(0);
	elem.setSource(1);
	vertFmt.addElement(elem);

	size_t numIndices = (patchHeight-1) * (patchWidth-1) * 6;
	m_pIndexBuffer = (short*)GetDEFAULTAllocator()->AllocateAligned(sizeof(short) * numIndices, 16 TD_ALLOC_SITE);
	float minY = 0, maxY = 0;

	for (size_t py = 0; py < patchHeight; ++py)
	{
		for (size_t px = 0; px < patchWidth; ++px)
		{
			size_t i = py * patchWidth + px;
			unsigned char h = pData[(py + yOff) * imgWidth + (px + xOff)];

			// calculate vert position and UV data
			m_pVertexBuffer[i].y = ((float)h * oo255) * pOwner->getZoneSize().y;
			m_pVertexBuffer[i].x = (float)px;
			m_pVertexBuffer[i].z = (float)py;

			minY = MathUtil::min(m_pVertexBuffer[i].y, minY);
			maxY = MathUtil::max(m_pVertexBuffer[i].y, maxY);
		}
	}

	for (size_t ny = 0; ny < patchHeight; ++ny)
	{
		for (size_t nx = 0; nx < patchWidth; ++nx)
		{
			//Vector4 n(Vector4::ZERO);
			//size_t i = ny + nx * patchWidth;

			if (ny < tileHeight)
			{
				// then we know there is one below this row
				//n += Vector4(xScale * m_pVertexBuffer[i].x, )
			}
		}
	}

	m_aabb.extents = Vector4(
		xScale, (maxY-minY), zScale, 0) / 2;

	Vector4 ulCorner(
		-pOwner->getZoneSize().x / 2 + (float)x * xScale,
		minY,
		-pOwner->getZoneSize().z / 2 + (float)y * zScale,
		0);

	m_aabb.center = m_aabb.extents + ulCorner;
	m_radius = m_aabb.getRadius();

	// TODO: normals

	// make triangle lists in the index buffer
	/**
		Terrain patch triangles look like this:

		0 - 1 - 2 - 3 ... 63
		|  /|  /|  /|
		| / | / | / |
		|/  |/  |/  |
	   64--65--66--67 ... 127

	   So the index marching order (CW winding) is

	   ( y*w + x )    ( (y+1)*w + x )   ( y*w + x+1 )
	   ( y*w + x+1 )  ( (y+1)*w + x )   ( (y+1)*w + x+1 )

	   where w = patch width

   */

	short* pI = m_pIndexBuffer;
	for (size_t py = 0; py < (patchHeight-1); ++py)
	{
		for (size_t px = 0; px < (patchWidth-1); ++px)
		{
			*pI++ = short(py		* patchWidth + px		); // 0
			*pI++ = short((py + 1)	* patchWidth + px		); // 64
			*pI++ = short(py		* patchWidth + px + 1	); // 1
			*pI++ = short((py + 1)	* patchWidth + px + 1	); // 64 
			*pI++ = short(py		* patchWidth + px + 1	); // 1
			*pI++ = short((py + 1)	* patchWidth + px		); // 65
		}
	}

	short* pIBuf = m_pIndexBuffer;
	TerrainVertex* pVBuf = m_pVertexBuffer;
	TerrainNormal* pNBuf = m_pNormalBuffer;

	Transform xform(
		ulCorner,
		Quaternion::IDENTITY,
		Vector4::UNIT_SCALE);
	setTransform(xform);
	inst.setTransform(xform);

	pOwner->addObject(this);

	RenderComponent* pRend = TD_NEW RenderComponent();
	pRend->setShaderName("TerrainShader/Main");
	pRend->setHost(this);
	pRend->setShadowCaster(pOwner->getCastShadows());
	pRend->setShadowReceiver(pOwner->getReceiveShadows());
	pRend->setMeshInstance(inst);
	m_components.insert(Components::value_type(pRend->getClassDef(), pRend));

	// create the actual mesh resource object
	GfxMesh* pMesh = AutoResPtr<GfxMesh>(m_hMesh);
	GfxSubMesh* pSubMesh = pMesh->createSubMesh();
	pSubMesh->setMaterial(pMtl);
	pSubMesh->setPrimitiveType(TRILIST);
	pSubMesh->setVertexFormat(Environment::get(), vertFmt);

	size_t streamIdx;
	// create the position buffer
	GfxVertexData* pVData = pSubMesh->createVertexData(
		streamIdx,
		Environment::get(),
		sizeof(TerrainVertex),
		numVerts, 
		(GfxVertexData::CreationFlags)(GfxVertexData::STATIC | GfxVertexData::WRITE_ONLY),
		pVBuf);

	if (!pVData)
	{
		return false;
	}

	// create the normal buffer
	pVData = pSubMesh->createVertexData(
		streamIdx,
		Environment::get(),
		sizeof(TerrainNormal),
		numVerts, 
		(GfxVertexData::CreationFlags)(GfxVertexData::STATIC | GfxVertexData::WRITE_ONLY),
		pNBuf);

	if (!pVData)
	{
		return false;
	}

	pSubMesh->createIndexData(
		Environment::get(),
		2, 
		numIndices, 
		pIBuf);

	// set up shader constant table in the render component
	GfxShaderConstantTable& constants = pRend->getShaderConstants();
	constants.begin();
	constants.addFloatConstant(String("xStep"));
	constants.addFloatConstant(String("zStep"));
	constants.addFloatConstant(String("uStart"));
	constants.addFloatConstant(String("vStart"));
	constants.addFloatConstant(String("uStep"));
	constants.addFloatConstant(String("vStep"));
	constants.addFloatConstant(String("xScale"));
	constants.addFloatConstant(String("zScale"));
	constants.end();

	// we should be able to set these and never touch them again...
	constants.getConstant(String("xStep"))->setValue(xStep);
	constants.getConstant(String("zStep"))->setValue(zStep);
	constants.getConstant(String("uStart"))->setValue(uStart);
	constants.getConstant(String("vStart"))->setValue(vStart);
	constants.getConstant(String("uStep"))->setValue(uStep);
	constants.getConstant(String("vStep"))->setValue(vStep);
	constants.getConstant(String("xScale"))->setValue(xScale);
	constants.getConstant(String("zScale"))->setValue(zScale);
	return true;
}
#endif
#if 0
//---------------------------------------------------------------------------
void TerrainPatch::preRender(GfxRenderer* pRenderer)
{
	/*	we want to set some of the vertex float registers specially for 
		terrain rendering, so we hook the pre-render callback for this 
		terrain	patch. We want to set the patch width and height (X and Z 
		dimensions) so that the vertex shader can scale the patch to the
		proper size (the mesh instance has the patch world space transform
		already).

		At present the only vertex register that the shader is using is the 
		WVP (16-19). All others should be available, but for the sake of 
		sanity we'll just use the bone constant registers (32+), since those 
		should never be in conflict with a terrain shader...
	*/

	float terrainConst[8];

	terrainConst[0] = m_xStep;
	terrainConst[1] = m_zStep;
	terrainConst[2] = m_uStart;
	terrainConst[3] = m_vStart;
	terrainConst[4] = m_uStep;
	terrainConst[5] = m_vStep;
	terrainConst[6] = m_xScale;
	terrainConst[7] = m_zScale;

	pRenderer->writeVertexFloatRegister(terrainConst, 40, 2);
}
#endif
//---------------------------------------------------------------------------
void TerrainPatch::notifyPropertyChangedLocal(
	const Reflection::PropertyDef* /*pPropDef*/)
{
}
