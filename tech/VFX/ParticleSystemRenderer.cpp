/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "ParticleSystemRenderer.h"
#include "ParticleSystem.h"
#include "Gfx/GfxRenderer.h"
#include "Gfx/GfxMesh.h"
#include "Gfx/GfxSubMesh.h"
#include "Gfx/GfxVertexData.h"
#include "Gfx/GfxVertexFormat.h"
#include "Gfx/GfxMaterial.h"
#include "Gfx/GfxTextureStage.h"
#include "Gfx/GfxTexture.h"
#include "Gfx/GfxUtil.h"
#include "Math/Matrix44.h"
#include "Math/MathUtil.h"
#include "Game/Zone.h"
#include "Util/Environment.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
TD_CLASS_IMPL(ParticleSystemRenderer);
//---------------------------------------------------------------------------
ParticleSystemRenderer::ParticleSystemRenderer()
{
	m_camPos = Vector4::ZERO;
}
//---------------------------------------------------------------------------
ParticleSystemRenderer::~ParticleSystemRenderer()
{

}
//---------------------------------------------------------------------------
void ParticleSystemRenderer::onInstanceCreated()
{
	setServerComponent(false); // not needed on a server
}
//---------------------------------------------------------------------------
bool ParticleSystemRenderer::initialize()
{
	// initialize the "reference" mesh quad
	GfxMesh* pMesh = m_inst.m_pProceduralMesh = TD_NEW GfxMesh;
	GfxSubMesh* pSubmesh = pMesh->createSubMesh();

	m_quad[0].pos = Vector4::ZERO;
	m_quad[1].pos = Vector4::ZERO;
	m_quad[2].pos = Vector4::ZERO;
	m_quad[3].pos = Vector4::ZERO;

	m_quad[0].pos.w = 1;
	m_quad[1].pos.w = 1;
	m_quad[2].pos.w = 1;
	m_quad[3].pos.w = 1;

	m_quad[1].pos.y = 1;
	m_quad[2].pos.x = 1;
	m_quad[3].pos.x = 1;
	m_quad[3].pos.y = 1;

	m_quad[0].texCoord.x = m_quad[0].pos.x;
	m_quad[0].texCoord.y = m_quad[0].pos.y;
	m_quad[1].texCoord.x = m_quad[1].pos.x;
	m_quad[1].texCoord.y = m_quad[1].pos.y;
	m_quad[2].texCoord.x = m_quad[2].pos.x;
	m_quad[2].texCoord.y = m_quad[2].pos.y;
	m_quad[3].texCoord.x = m_quad[3].pos.x;
	m_quad[3].texCoord.y = m_quad[3].pos.y;

	m_quadIndices[0] = 0;
	m_quadIndices[1] = 1;
	m_quadIndices[2] = 2;
	m_quadIndices[3] = 1;
	m_quadIndices[4] = 2;
	m_quadIndices[5] = 3;

	GfxVertexFormat fmt;
	GfxVertexFormat::Element elem;
	elem.offset = 0;
	elem.semantic = POSITION;
	elem.type = FLOAT4;
	elem.setUsage(0);
	elem.setSource(0);
	fmt.addElement(elem);
	elem.type = FLOAT2;
	elem.semantic = TEXCOORD;
	elem.offset += GfxUtil::getSizeOf(FLOAT4);
	fmt.addElement(elem);

	size_t stream;
	GfxVertexData* pVData = pSubmesh->createVertexData(
		stream, 
		Environment::get(),
		fmt.getVertexSize(),
		4,
		(GfxVertexData::CreationFlags)(GfxVertexData::STATIC | GfxVertexData::WRITE_ONLY),
		m_quad);

	GfxIndexData* pIData = pSubmesh->createIndexData(
		Environment::get(),
		2,
		6,
		m_quadIndices
		);

	pSubmesh->setPrimitiveType(TRISTRIP);

	// setup a transparent material, with texture if specified
	GfxMaterial* pMtl = TD_NEW GfxMaterial;
	if (getTexture().length())
	{
		String texPath(getAssetRootPath());
		texPath += "/";
		texPath += getTexture();

		GfxTextureStage* pStage = pMtl->addTextureStage();
//		pStage->setTexture(0,
//			Environment::get().pResourceMgr->acquire(GfxTexture::RESOURCE_TYPE, texPath));
		pStage->setEnabled(true);
		pStage->setBlendMode(GfxTextureStage::BLENDMODE_MODULATE);
		pStage->setMinFilter(GfxTextureStage::FILTER_BILINEAR);
		pStage->setMagFilter(GfxTextureStage::FILTER_BILINEAR);
		pStage->setMipMapFilter(GfxTextureStage::FILTER_BILINEAR);
	}

	pMtl->setDepthWrite(false);
	pMtl->setAlphaBlending(true);
	pSubmesh->setMaterial(pMtl, true);

	// set up the instanced-rendering data
	m_inst.m_instanceVertexData = Environment::get().pRenderer->createVertexData();

	// vertex format adds just the transform (4x4) for now
	GfxVertexFormat::Element instElem;
	instElem.offset = 0;
	instElem.semantic = TEXCOORD;
	instElem.type = FLOAT4;
	instElem.setUsage(1);
	instElem.setSource(1);
	fmt.addElement(instElem);
	instElem.offset += GfxUtil::getSizeOf(FLOAT4);
	instElem.setUsage(2);
	fmt.addElement(instElem);
	instElem.offset += GfxUtil::getSizeOf(FLOAT4);
	instElem.setUsage(3);
	fmt.addElement(instElem);
	instElem.offset += GfxUtil::getSizeOf(FLOAT4);
	instElem.setUsage(4);
	fmt.addElement(instElem);

	pSubmesh->setVertexFormat(Environment::get(), fmt);
	
	return true;
}
//---------------------------------------------------------------------------
bool ParticleSystemRenderer::destroy()
{
	// delete everything we own; note that the memory pool cleans itself up
	if (m_inst.m_pProceduralMesh)
		m_inst.m_pProceduralMesh->destroy();
	delete m_inst.m_pProceduralMesh;
	m_inst.m_pProceduralMesh = 0;

	m_inst.m_instanceCount = 0;

	if (m_inst.m_instanceVertexData)
		m_inst.m_instanceVertexData->destroy();
	delete m_inst.m_instanceVertexData;
	m_inst.m_instanceVertexData = 0;

	return true;
}
//---------------------------------------------------------------------------
bool ParticleSystemRenderer::update(float deltaT)
{
	// we only work with ParticleSystem as a host
	if (!m_pHost->isOfType(ParticleSystem::getClassDef()))
		return false;

	ParticleSystem* pSystem = static_cast<ParticleSystem*>(m_pHost);

	// once everything is updated, assemble the instanced rendering data
	m_inst.m_instanceCount = m_particles.size();
	GfxVertexData* pData = m_inst.m_instanceVertexData;
	pData->release();
	pData->initialize(
		(GfxVertexData::CreationFlags)(GfxVertexData::DYNAMIC|GfxVertexData::WRITE_ONLY), 
		sizeof(Matrix44), 
		m_particles.size());

	Matrix44* pBuf = 0;
	if (pData)
		pBuf = (Matrix44*)pData->lock();
	if (!pBuf && pData)
		pBuf = (Matrix44*)pData->getBuffer();

	// we need to populate the buffer with activeParticleCount 4x4 transform
	// instances. In order to get the correct transform, we need to have the
	// particles "look at" the camera. We have the particle's world-space 
	// position, we have its upvector (UNIT Y) and we have the target (the
	// camera's world-space position). And we have a Matrix4 method to give us
	// the transform we want.
	// We also need to update the particle system's world AABB
	Vector4 cornerMin = Vector4::POSITIVE_INFINITY;
	Vector4 cornerMax = Vector4::NEGATIVE_INFINITY;

	for (ParticleList::iterator p = m_particles.begin(); p != m_particles.end(); ++p)
	{
		if (pBuf)
		{
			Vector4 worldPos((*p)->m_position + m_pHost->getTransformWS().trans);
			Vector4 z(m_camPos - worldPos);
			z.normalize(); // this is our orthonormal Z axis

			// Y axis is based on camera's local Y axis
			Vector4 y(m_camRot * Vector4::UNIT_Y);

			// X axis is the cross of these two
			Vector4 x(cross(z, y));
			x.normalize();

			y = cross(x, z);

			Matrix44 mat = Matrix44::IDENTITY;

			mat.m[0][0] = x.x;    mat.m[1][0] = x.y;     mat.m[2][0] = x.z;
			mat.m[0][1] = y.x;    mat.m[1][1] = y.y;     mat.m[2][1] = y.z;
			mat.m[0][2] = z.x;    mat.m[1][2] = z.y;     mat.m[2][2] = z.z;

			mat.m[3][0] = worldPos.x; // -dot(worldPos, x);
			mat.m[3][1] = worldPos.y; // -dot(worldPos, y);
			mat.m[3][2] = worldPos.z; // -dot(worldPos, z);
			mat.m[3][3] = 1;

			*pBuf++ = mat;
		}

		// expand particle system AABB
		cornerMin = MathUtil::makeMin(cornerMin, (*p)->m_position);
		cornerMax = MathUtil::makeMax(cornerMax, (*p)->m_position);
	}

	if (pData)
		pData->unlock();

	AABB aabb;
	aabb.fromCorners(cornerMin, cornerMax);
	aabb.center += m_pHost->getTransformWS().trans; // center on our location
	m_pHost->setBoundingBox(aabb);

	m_inst.setTransform(m_pHost->getTransformWS());

	return true;
}
