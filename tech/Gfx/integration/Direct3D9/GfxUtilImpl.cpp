/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "GfxRenderer.h"
#include "GfxRendererD3D9.h"
#include "GfxMesh.h"
#include "GfxSubMesh.h"
#include "GfxVertexFormat.h"
#include "GfxVertexData.h"
#include "GfxIndexData.h"
#include "GfxMeshInstance.h"
#include "GfxTexture.h"
#include "GfxCamera.h"
#include "GfxUtil.h"
#include "Memory/Memory.h"
#include "Memory/Allocators.h"
#include "Math/Transform.h"
#include "Math/Quaternion.h"
#include "Math/Vector4.h"
#include "Util/Environment.h"
#include <assert.h>
#include <new.h>

using namespace Teardrop;
//---------------------------------------------------------------------------
bool createTeapotImpl(GfxMesh& rMesh, Environment& env)
{
	GfxRendererD3D9* pRI = static_cast<GfxRendererD3D9*>(env.pRenderer);
	ID3DXMesh* pMesh;
	D3DXCreateTeapot(
		pRI->getDevice(),
		&pMesh,
		0);

	LPDIRECT3DINDEXBUFFER9 ppIB;
	LPDIRECT3DVERTEXBUFFER9 ppVB;
	D3DINDEXBUFFER_DESC ibDesc;
	D3DVERTEXBUFFER_DESC vbDesc;
	void *pIData, *pVData;

	// extract index info about the teapot
	HRESULT hr = pMesh->GetIndexBuffer(&ppIB);
	hr = ppIB->GetDesc(&ibDesc);
	size_t indexSize = (ibDesc.Format==D3DFMT_INDEX16) ? 2 : 4;
	size_t indexCount = ibDesc.Size / indexSize;

	// extract vertex info about the teapot -- its FVF is XYZ | NORMAL
	hr = pMesh->GetVertexBuffer(&ppVB);
	hr = ppVB->GetDesc(&vbDesc);
	size_t vertexSize = 24; // we know it's XYZ|NORMAL, or 6 floats
	size_t vertexCount = vbDesc.Size / vertexSize;

	GfxSubMesh* pSubMesh = rMesh.createSubMesh();

	// set the index data
	hr = ppIB->Lock(0, ibDesc.Size, &pIData, 0);
	GfxIndexData* pIndexData = pSubMesh->createIndexData(
		env, indexSize, indexCount);
	void* dest = pIndexData->lock();
	memcpy(dest, pIData, ibDesc.Size);
	pIndexData->unlock();
	hr = ppIB->Unlock();
	ppIB->Release();

	// set the vertex data
	hr = ppVB->Lock(0, vbDesc.Size, &pVData, 0);
	size_t streamIndex;
	GfxVertexData* pVertexData = pSubMesh->createVertexData(
		streamIndex, env, vertexSize, vertexCount, GfxVertexData::WRITE_ONLY);
	dest = pVertexData->lock();
	memcpy(dest, pVData, vbDesc.Size);
	pVertexData->unlock();
	hr = ppVB->Unlock();
	ppVB->Release();
	pSubMesh->setPrimitiveType(TRILIST);

	// set the vertex format
	GfxVertexFormat fmt;
	GfxVertexFormat::Element elem;
	elem.offset = 0;
	elem.semantic = POSITION;
	elem.type = FLOAT3;
	elem.setSource(0);
	elem.setUsage(0);
	fmt.addElement(elem);
	elem.offset += (unsigned char)GfxUtil::getSizeOf((VertexElementType)elem.type);
	elem.semantic = NORMAL;
	fmt.addElement(elem);
	pSubMesh->setVertexFormat(env, fmt);

	pMesh->Release();

	return true;
}
//---------------------------------------------------------------------------
static float aabbVData[] = 
{
	-50.5f,	50.5f,	-50.5f, 0,
	 50.5f,	50.5f,	-50.5f, 0,
	 50.5f, -50.5f,	-50.5f, 0,
	-50.5f, -50.5f,	-50.5f, 0,
	-50.5f,	50.5f,	 50.5f, 0,
	 50.5f,	50.5f,	 50.5f, 0,
	 50.5f, -50.5f,	 50.5f, 0,
	-50.5f, -50.5f,	 50.5f, 0,
};

static short aabbIData[] = 
{
	0, 1,
	1, 2,
	2, 3,
	3, 0,
	0, 4,
	4, 5,
	5, 6,
	6, 7,
	7, 4,
	1, 5,
	2, 6,
	3, 7,
};
//---------------------------------------------------------------------------
GfxMesh* GfxUtil::createAABBTemplateMesh(GfxRenderer* /*pRend*/)
{
	// 1 submesh (linelist), 8 verts, 12 lines, all white, solid
	// centered at 0,0,0 with extents of 0.5 each direction for a unit cube,
	// to allow for scaling by the AABB extents at render time

	// material will be owned by the submesh so it will be destroyed when
	// the mesh is destroyed
	GfxMaterial* pMtl = TD_NEW GfxMaterial;
	pMtl->initialize();
	pMtl->setDiffuse(0xFFFFFFFF);
	pMtl->setNumLights(0);
	pMtl->setVertexColors(false);

	GfxMesh* pMesh = TD_NEW GfxMesh;
	pMesh->initialize();
	GfxSubMesh* pSM = pMesh->createSubMesh();

	GfxVertexFormat fmt;
	GfxVertexFormat::Element elem;
	elem.semantic = POSITION;
	elem.offset = 0;
	elem.type = FLOAT4;
	elem.usage_source = 0;
	fmt.addElement(elem);

	size_t stream;
	pSM->createVertexData(
		stream,
		Environment::get(),
		fmt.getVertexSize(),
		8,
		(GfxVertexData::CreationFlags)(GfxVertexData::STATIC | GfxVertexData::WRITE_ONLY),
		aabbVData);
	pSM->setPrimitiveType(LINELIST);

	pSM->createIndexData(
		Environment::get(),
		2,
		24,
		aabbIData);

	pSM->setMaterial(pMtl, true);
	pSM->setVertexFormat(Environment::get(), fmt);

	return pMesh;
}