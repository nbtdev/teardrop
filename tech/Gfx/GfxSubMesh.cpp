/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/


#include "GfxRenderer.h"
#include "GfxSubMesh.h"
#include "GfxIndexData.h"
#include "GfxMaterial.h"
#include "GfxCommon.h"
#include "Util/Environment.h"
#include "Math/Matrix44.h"
#include <assert.h>
#include <memory.h>

using namespace Teardrop;
//---------------------------------------------------------------------------
GfxSubMesh::GfxSubMesh()
{
	memset(this, 0, sizeof(GfxSubMesh));
	m_hashCode = 0;
	m_primType = UNKNOWN;
}
//---------------------------------------------------------------------------
// placement c'tor
GfxSubMesh::GfxSubMesh(int /*i*/)
{
	m_hashCode = 0;
	m_primType = UNKNOWN;
}
//---------------------------------------------------------------------------
GfxSubMesh::~GfxSubMesh()
{
}
//---------------------------------------------------------------------------
bool GfxSubMesh::initialize()
{
	recalcHashCode();
	return true;
}
//---------------------------------------------------------------------------
// used by resource manager to release dynamic data from static resources
bool GfxSubMesh::release()
{
	// release our vertex and index data
	for (size_t i=0; i<m_pVertexData.size(); ++i)
	{
        m_pVertexData[i]->release();
	}

	if (m_pIndexData)
		m_pIndexData->release();

	m_format.release();

	// textures are handled separately by the resource manager and their
	// release will be called elsewhere

	return true;
}
//---------------------------------------------------------------------------
bool GfxSubMesh::destroy()
{
	// clean up any dynamically-created vertex/index streams
	for (size_t i=0; i<8; ++i)
	{
		unsigned char f = (0x01 << (unsigned char)i);

		if ((f & m_dynamicFlags) == f)
		{
			// the high-order bit is for the index data pointer
			if (f == 0x80)
			{
				if (m_pIndexData)
				{
					m_pIndexData->destroy();
					delete m_pIndexData;
					m_pIndexData = 0;
				}
			}
			else if(f == 0x40)
			{
				// the second-highest bit is for the material
				if (m_pMaterial)
				{
					m_pMaterial->destroy();
					m_pMaterial = 0;
				}
			}
			else
			{
				GfxVertexData* p = m_pVertexData[i];
				m_pVertexData[i] = 0;

				if (p)
				{
					p->destroy();
					delete p;
					p = 0;
				}
			}
		}

		// regardless, zero out this bit -- if it was already zero, no 
		// change, and if it was set, it needs to be cleared
		m_dynamicFlags &= ~f;
	}

	// release any vertex format implementation resources
	m_format.release();
	m_hashCode = 0;

	return true;
}
//---------------------------------------------------------------------------
const GfxVertexData* const GfxSubMesh::getVertexData(size_t stream) const
{
	assert(stream < m_pVertexData.size());
	if (stream >= m_pVertexData.size())
	{
		return 0;
	}

	return m_pVertexData[stream];
}
//---------------------------------------------------------------------------
size_t GfxSubMesh::getNumVertexData()
{
	return m_pVertexData.size();
}
//---------------------------------------------------------------------------
GfxVertexData* GfxSubMesh::getVertexData(size_t stream)
{
	assert(stream < m_pVertexData.size());
	if (stream >= m_pVertexData.size())
	{
		return 0;
	}

	return m_pVertexData[stream];
}
//---------------------------------------------------------------------------
const GfxIndexData* const GfxSubMesh::getIndexData() const
{
	return m_pIndexData;
}
//---------------------------------------------------------------------------
GfxIndexData* GfxSubMesh::getIndexData()
{
	return m_pIndexData;
}
//---------------------------------------------------------------------------
bool GfxSubMesh::usesSharedVertexData() const
{
	return m_bUseSharedVertexData;
}
//---------------------------------------------------------------------------
void GfxSubMesh::setUseSharedVertexData(bool bUseSharedData)
{
	m_bUseSharedVertexData = bUseSharedData;
}
//---------------------------------------------------------------------------
void GfxSubMesh::setPrimitiveType(PrimitiveType type)
{
	m_primType = (unsigned char)type;
}
//---------------------------------------------------------------------------
bool GfxSubMesh::setVertexData(
	const GfxVertexData* /*vertexData*/, size_t /*stream*/)
{
	// NOT IMPLEMENTED YET
	assert(false);
	return false;
}
//---------------------------------------------------------------------------
bool GfxSubMesh::setIndexData(const GfxIndexData* /*indexData*/)
{
	// NOT IMPLEMENTED YET
	assert(false);
	return false;
}
//---------------------------------------------------------------------------
GfxMaterial* GfxSubMesh::getMaterial()
{
	return m_pMaterial;
}
//---------------------------------------------------------------------------
void GfxSubMesh::setMaterial(GfxMaterial* pMaterial, bool autoDelete)
{
	m_pMaterial = pMaterial;

	if (autoDelete)
	{
		m_dynamicFlags |= 0x40; // mark this for destruction
	}
}
//---------------------------------------------------------------------------
GfxVertexData* GfxSubMesh::createVertexData(
	size_t& streamIndex,
	Environment& env,
	size_t vertexSize,
	size_t vertexCount, 
	GfxVertexData::CreationFlags flags,
	void* pData)
{
	if (m_pVertexData.size() < MAX_VERTEX_STREAMS)
	{
		streamIndex = m_pVertexData.size();
		m_pVertexData.push_back(env.pRenderer->createVertexData());

		// mark the dynamic flags so we know we have to delete this one
		m_dynamicFlags |= (0x01 << streamIndex);

		// initialize this instance
		m_pVertexData[streamIndex]->initialize(
			flags, 
			(unsigned int)vertexSize, 
			(unsigned int)vertexCount, 
			pData);

		// calculate this submesh's hash code (based on vertex layout)
		recalcHashCode();

		// return the new vertex data
		return m_pVertexData[streamIndex];
	}

	return 0;
}
//---------------------------------------------------------------------------
GfxIndexData* GfxSubMesh::createIndexData(
	Environment& env,
	size_t indexSize, 
	size_t indexCount, 
	void* pData)
{
	unsigned char mask = 0x80;
	if (m_pIndexData)
	{
		if ((m_dynamicFlags & mask) == mask)
		{
			m_pIndexData->destroy();
			m_pIndexData->~GfxIndexData();
			GFX_FREE(m_pIndexData);
		}
	}

	m_pIndexData = env.pRenderer->createIndexData();

	// mark the dynamic flags so we know we have to delete this one
	m_dynamicFlags |= mask;

	m_pIndexData->initialize(
		(unsigned int)indexSize, 
		(unsigned int)indexCount, 
		pData);
	return m_pIndexData;
}
//---------------------------------------------------------------------------
bool GfxSubMesh::setVertexFormat(Environment& env, const GfxVertexFormat& fmt)
{
	// release any existing implementation-dependent data
	m_format.release();

	m_format = fmt;
	recalcHashCode();

	if (!env.isOffline)
	{
		// then fix up with incoming format data
		m_format.fix(env);
	}

	return true;
}
//---------------------------------------------------------------------------
void GfxSubMesh::recalcHashCode()
{
	/* 
		hash is based entirely on what is in the vertex format; we can only
		have 8 elements per vertex, but what those are (other than POSITION
		is required) is entirely arbitrary, so in order to make a code that
		is unique to what the format describes, we need to consider semantics
		as well as usages. There are only two semantics that can have usages
		other than 0 -- color and texcoord -- so we can just create a simple
		bitfield, where the LSB is a bitmask of the semantics used, the next
		LSB is the texcoord usages, and the next LSB is the color usages. So, 
		the following bitfield means "position, normal, diffuse and specular
		colors, and three texcoord sets":

		00000000 00000011 00000111 00001111

		This hash is only used for fast lookup of vertex shader (the top 12  
		bits are reserved for the shader manager, for different shader 
		permutations that use the same vertex format).
	*/

	unsigned int hash = 0;

	size_t numElems = m_format.getNumElements();
	for (size_t i=0; i<numElems; ++i)
	{
		GfxVertexFormat::Element& elem = m_format.getElementAt(i);
		hash |= (unsigned int)(1 << (elem.semantic-1));

		if (elem.semantic == TEXCOORD)
		{
			hash |= (unsigned int)((1 << elem.getUsage()) << 8);
		}

		if (elem.semantic == COLOR)
		{
			hash |= (unsigned int)((1 << elem.getUsage()) << 16);
		}
	}

	m_hashCode = hash;
}
//---------------------------------------------------------------------------
PrimitiveType GfxSubMesh::getPrimitiveType() const
{
	return (PrimitiveType)m_primType;
}
//---------------------------------------------------------------------------
const Matrix44** GfxSubMesh::getBindPoses() const
{
    return const_cast<const Matrix44**>(m_pBindPoseData.data());
}
