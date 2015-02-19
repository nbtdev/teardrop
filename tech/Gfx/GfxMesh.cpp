/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/


#include "GfxRenderer.h"
#include "GfxMesh.h"
#include "GfxSubMesh.h"
#include "GfxCommon.h"
#include "Util/Environment.h"
#include "Math/MathUtil.h"
#include <assert.h>
#include <memory.h>

#if defined(_WIN32) || defined(_WIN64)
    #define STRCPY(d, n, s) strcpy_s(d, n, s)
#else // _WIN32, _WIN64
    #define STRCPY(d, n, s) strncpy(d, s, n)
#endif // _WIN32, _WIN64

using namespace Teardrop;
//---------------------------------------------------------------------------
GfxMesh::GfxMesh()
{
	memset((char*)this+8, 0, sizeof(GfxMesh)-8);
}
//---------------------------------------------------------------------------
// placement c'tor
GfxMesh::GfxMesh(int /*i*/)
{
}
//---------------------------------------------------------------------------
GfxMesh::~GfxMesh()
{
}
//---------------------------------------------------------------------------
bool GfxMesh::initialize()
{
	return true;
}
//---------------------------------------------------------------------------
// called by resource manager to clean up non-dynamically-created resource data
bool GfxMesh::release()
{
    for (size_t i=0; i<mSubMeshes.size(); ++i)
	{
        mSubMeshes[i]->release();
	}

	return true;
}
//---------------------------------------------------------------------------
bool GfxMesh::destroy()
{
	// clean up all submeshes we created
    for (size_t i=0; i<mSubMeshes.size(); ++i)
	{
		size_t flag = (0x01 << i);
		if ((m_dynamicSubmeshFlags & flag) == flag)
		{
            GfxSubMesh* p = mSubMeshes[i];
			if (p)
			{
				p->destroy();
				delete p;
                mSubMeshes[i] = nullptr;

				// clear the dynamic bit for this submesh
				m_dynamicSubmeshFlags &= ~flag;
			}
		}
	}

	// same for the vertex data streams (if any)
    for (size_t i=0; i<mSharedVertexData.size(); ++i)
	{
		unsigned char flag = (0x01 << (unsigned char)i);
		if ((m_dynamicSharedVertexFlags & flag) == flag)
		{
            GfxVertexData* p = mSharedVertexData[i];
			if (p)
			{
				p->destroy();
				delete p;
                mSharedVertexData[i] = 0;

				// clear the dynamic bit for this submesh
				m_dynamicSharedVertexFlags &= ~flag;
			}
		}
	}

	return true;
}
//---------------------------------------------------------------------------
GfxSubMesh* GfxMesh::createSubMesh(bool bUseSharedVertexData)
{
	// find the first available slot and put a new one there
	GfxSubMesh* p = TD_NEW GfxSubMesh;
	if (!p->initialize())
	{
		return 0;
	}

	p->setUseSharedVertexData(bUseSharedVertexData);
    mSubMeshes.push_back(p);

	// mark this submesh entry as dynamic (for cleanup later)
    m_dynamicSubmeshFlags |= (0x01 << (mSubMeshes.size()-1));
	return p;
}
//---------------------------------------------------------------------------
bool GfxMesh::destroySubMesh(size_t /*index*/)
{
	// NOT IMPLEMENTED
	assert(false);
	return false;
}
//---------------------------------------------------------------------------
bool GfxMesh::destroySubMesh(GfxSubMesh* /*pSubMesh*/)
{
	// NOT IMPLEMENTED
	assert(false);
	return false;
}
//---------------------------------------------------------------------------
GfxSubMesh* GfxMesh::getSubMesh(size_t index) const
{
    return mSubMeshes[index];
}
//---------------------------------------------------------------------------
size_t GfxMesh::getNumSubMeshes() const
{
    return mSubMeshes.size();
}
//---------------------------------------------------------------------------
size_t GfxMesh::getNumSharedVertexData() const
{
    return mSharedVertexData.size();
}
//---------------------------------------------------------------------------
const GfxVertexData* const GfxMesh::getSharedVertexData(size_t index) const
{
	assert(index < MAX_SHARED_VERTEX_STREAMS);
	if (index >= MAX_SHARED_VERTEX_STREAMS)
	{
		return 0;
	}

    return mSharedVertexData[index];
}
//---------------------------------------------------------------------------
GfxVertexData* GfxMesh::getSharedVertexData(size_t index)
{
	assert(index < MAX_SHARED_VERTEX_STREAMS);
	if (index >= MAX_SHARED_VERTEX_STREAMS)
	{
		return 0;
	}

    return mSharedVertexData[index];
}
//---------------------------------------------------------------------------
GfxVertexData* GfxMesh::createSharedVertexData(
	size_t& /*out*/streamIndex,
	Environment& env,
	GfxVertexData::CreationFlags flags,
	size_t elemSize, 
	size_t numElems, 
	void* pData)
{
	// find the first available slot and put a new one there
    if (mSharedVertexData.size() < MAX_SHARED_VERTEX_STREAMS)
	{
        streamIndex = mSharedVertexData.size();
        mSharedVertexData.push_back(env.pRenderer->createVertexData());

		// mark this one as dynamic for later cleanup
		m_dynamicSharedVertexFlags |= (0x01 << streamIndex);

        if (!mSharedVertexData[streamIndex]->initialize(
			flags, 
			(unsigned int)elemSize, 
			(unsigned int)numElems, 
			pData))
		{
			return 0;
		}

        return mSharedVertexData[streamIndex];
	}

	// if no available slots, report the bad news to the caller
	return 0;
}
//---------------------------------------------------------------------------
const char* GfxMesh::getName() const
{
	return m_name;
}
//---------------------------------------------------------------------------
void GfxMesh::setName(const char* name)
{
    STRCPY(m_name, MAX_MESH_NAME_LENGTH, name);
}
//---------------------------------------------------------------------------
bool GfxMesh::clear()
{
	return destroy() && initialize();
}
//---------------------------------------------------------------------------
const GfxVertexFormat& GfxMesh::getSharedVertexFormat() const
{
	return m_sharedVertexFormat;
}
//---------------------------------------------------------------------------
void GfxMesh::setSharedVertexFormat(const GfxVertexFormat& fmt)
{
	m_sharedVertexFormat = fmt;
}
//---------------------------------------------------------------------------
void GfxMesh::getAABB(Vector4& minCorner, Vector4& maxCorner) const
{
	minCorner.x = 
	minCorner.y = 
	minCorner.z = 100000;
	maxCorner.x = 
	maxCorner.y = 
	maxCorner.z = -100000;
	minCorner.w = 
	maxCorner.w = 0;

	// go through each vertex in the mesh and push out the corners as needed
    for (size_t i=0; i<mSubMeshes.size(); ++i)
	{
        GfxSubMesh* pSM = mSubMeshes[i];

		// go through each vertex position in the submesh
		const GfxVertexFormat::Element& elem =
			pSM->getVertexFormat().getElementBySemantic(POSITION);

		GfxVertexData* pVD = pSM->getVertexData((size_t)elem.getSource());
		char* pData = (char*)pVD->lock();

		if (!pData)
		{
			pData = (char*)pVD->getBuffer();

			if (!pData)
			{
				// give up
				minCorner = 
				maxCorner = Vector4::ZERO;
				return;
			}
		}

		unsigned int offset = 0;
		for (unsigned int j=0; j<pVD->getVertexCount(); ++j)
		{
			float* pPos = (float*)(pData + offset);
			offset += pVD->getVertexSize();

			minCorner.x = MathUtil::min(minCorner.x, pPos[0]);
			minCorner.y = MathUtil::min(minCorner.y, pPos[1]);
			minCorner.z = MathUtil::min(minCorner.z, pPos[2]);
			maxCorner.x = MathUtil::max(maxCorner.x, pPos[0]);
			maxCorner.y = MathUtil::max(maxCorner.y, pPos[1]);
			maxCorner.z = MathUtil::max(maxCorner.z, pPos[2]);
		}

		pVD->unlock();
	}
}
