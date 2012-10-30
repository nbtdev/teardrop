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

#include "stdafx.h"
#include "GfxRenderer.h"
#include "GfxMesh.h"
#include "GfxSubMesh.h"
#include "GfxCommon.h"
#include "Util/include/Environment.h"
#include "Util/include/FourCC.h"
#include "Math/include/MathUtil.h"
#include "Serialization/include/ResourceSerializer.h"
#include <assert.h>
#include <memory.h>
#include <new.h>

using namespace CoS;
//---------------------------------------------------------------------------
DEFINE_SERIALIZABLE(GfxMesh);
const FourCC& GfxMesh::RESOURCE_TYPE = FourCC('M','S','H',' ');
//---------------------------------------------------------------------------
GfxMesh::GfxMesh()
{
	memset((char*)this+8, 0, sizeof(GfxMesh)-8);
}
//---------------------------------------------------------------------------
// placement c'tor
GfxMesh::GfxMesh(int i) 
{
	UNREFERENCED_PARAMETER(i);
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
	for (size_t i=0; i<m_pSubMeshes.size(); ++i)
	{
		m_pSubMeshes[i]->release();
	}

	return true;
}
//---------------------------------------------------------------------------
bool GfxMesh::destroy()
{
	// clean up all submeshes we created
	for (size_t i=0; i<m_pSubMeshes.size(); ++i)
	{
		size_t flag = (0x01 << i);
		if ((m_dynamicSubmeshFlags & flag) == flag)
		{
			GfxSubMesh* p = m_pSubMeshes[i];
			if (p)
			{
				p->destroy();
				delete p;
				m_pSubMeshes[i] = 0;

				// clear the dynamic bit for this submesh
				m_dynamicSubmeshFlags &= ~flag;
			}
		}
	}

	// same for the vertex data streams (if any)
	for (size_t i=0; i<m_pSharedVertexData.size(); ++i)
	{
		unsigned char flag = (0x01 << (unsigned char)i);
		if ((m_dynamicSharedVertexFlags & flag) == flag)
		{
			GfxVertexData* p = m_pSharedVertexData[i];
			if (p)
			{
				p->destroy();
				delete p;
				m_pSharedVertexData[i] = 0;

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
	GfxSubMesh* p = COS_NEW GfxSubMesh;
	if (!p->initialize())
	{
		return 0;
	}

	p->setUseSharedVertexData(bUseSharedVertexData);
	m_pSubMeshes.push_back(p);

	// mark this submesh entry as dynamic (for cleanup later)
	m_dynamicSubmeshFlags |= (0x01 << (m_pSubMeshes.size()-1));
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
	return m_pSubMeshes[index];
}
//---------------------------------------------------------------------------
size_t GfxMesh::getNumSubMeshes() const
{
	return m_pSubMeshes.size();
}
//---------------------------------------------------------------------------
size_t GfxMesh::getNumSharedVertexData() const
{
	return m_pSharedVertexData.size();
}
//---------------------------------------------------------------------------
const GfxVertexData* const GfxMesh::getSharedVertexData(size_t index) const
{
	assert(index < MAX_SHARED_VERTEX_STREAMS);
	if (index >= MAX_SHARED_VERTEX_STREAMS)
	{
		return 0;
	}

	return m_pSharedVertexData[index];
}
//---------------------------------------------------------------------------
GfxVertexData* GfxMesh::getSharedVertexData(size_t index)
{
	assert(index < MAX_SHARED_VERTEX_STREAMS);
	if (index >= MAX_SHARED_VERTEX_STREAMS)
	{
		return 0;
	}

	return m_pSharedVertexData[index];
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
	if (m_pSharedVertexData.size() < MAX_SHARED_VERTEX_STREAMS)
	{
		streamIndex = m_pSharedVertexData.size();
		m_pSharedVertexData.push_back(env.pRenderer->createVertexData());

		// mark this one as dynamic for later cleanup
		m_dynamicSharedVertexFlags |= (0x01 << streamIndex);

		if (!m_pSharedVertexData[streamIndex]->initialize(
			flags, 
			(unsigned int)elemSize, 
			(unsigned int)numElems, 
			pData))
		{
			return 0;
		}

		return m_pSharedVertexData[streamIndex];
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
	strcpy_s(m_name, MAX_MESH_NAME_LENGTH, name);
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
	for (size_t i=0; i<m_pSubMeshes.size(); ++i)
	{
		GfxSubMesh* pSM = m_pSubMeshes[i];

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
//---------------------------------------------------------------------------
bool GfxMesh::serialize(ResourceSerializer& serializer)
{
	// save the dynamic flags temporarily; memory image has no dynamic data
	m_dynamicSubmeshFlags = 0;
	m_dynamicSharedVertexFlags = 0;

	serializer.start();
	serializer.relocate(GfxMesh::getSerialClass()->getId());
	serializer.relocate(this, sizeof(GfxMesh));

	// relocate our pointer arrays
	serializer.relocate(m_pSubMeshes);
	serializer.relocate(m_pSharedVertexData);

	// then our vertex data, if any
	for (size_t i=0; i<m_pSharedVertexData.size(); ++i)
	{
		m_pSharedVertexData[i]->serialize(serializer);
	}

	// and finally our submeshes
	for (size_t i=0; i<m_pSubMeshes.size(); ++i)
	{
		m_pSubMeshes[i]->serialize(serializer);
	}

	return serializer.finalize();
}
