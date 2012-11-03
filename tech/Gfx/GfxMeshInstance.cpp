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
#include "GfxMesh.h"
#include "GfxSubMesh.h"
#include "GfxMaterial.h"
#include "GfxMeshInstance.h"
#include "GfxRenderer.h"
#include "Math/Matrix44.h"
#include "Resource/ResourceManager.h"
#include "Util/Environment.h"
#include "Memory/Allocators.h"
#include <new.h>
#include <limits>

using namespace CoS;
//---------------------------------------------------------------------------
GfxMeshInstance::GfxMeshInstance()
{
	m_meshHandle = INVALID_RESOURCE_HANDLE;
	m_pMaterialOverrides = 0;
	m_numMaterials = 0;
	m_pMatrixPalette = 0;
	m_numMatrices = 0;
	m_pLights = 0;
	m_numLights = 0;
	m_pShaderConstants = 0;
	m_pShader = 0;
	m_bCastShadows = 1;
	m_bReceiveShadows = 1;
	m_distToCamera = std::numeric_limits<float>::infinity();
	m_bShowAABB = 0;
	m_pProceduralMesh = 0;
	m_instanceCount = 0;
	m_instanceVertexData = 0;
}
//---------------------------------------------------------------------------
GfxMeshInstance::~GfxMeshInstance()
{
}
//---------------------------------------------------------------------------
bool GfxMeshInstance::initialize()
{
	return true;
}
//---------------------------------------------------------------------------
bool GfxMeshInstance::destroy()
{
	if (m_pMaterialOverrides)
	{
		GFX_FREE(m_pMaterialOverrides);
		m_pMaterialOverrides = 0;
		m_numMaterials = 0;
	}

	// TODO: release mesh by handle back to the pool?
	if (m_meshHandle != INVALID_RESOURCE_HANDLE)
	{
		Environment::get().pResourceMgr->release(m_meshHandle);
	}

	return true;
}
//---------------------------------------------------------------------------
GfxMaterial* GfxMeshInstance::getMaterialByIndex(size_t index) const
{
	if (index >= m_numMaterials)
	{
		return 0;
	}

	return m_pMaterialOverrides[index];
}
//---------------------------------------------------------------------------
bool GfxMeshInstance::setMeshHandle(HResource handle)
{
	m_meshHandle = handle;
	return true;
}
//---------------------------------------------------------------------------
bool GfxMeshInstance::setMaterialByIndex(
	size_t index, GfxMaterial* pMaterial)
{
	if (index >= m_numMaterials)
	{
		return false;
	}

	m_pMaterialOverrides[index] = pMaterial;
	return true;
}
//---------------------------------------------------------------------------
void GfxMeshInstance::createMaterialOverrides(size_t numMaterials)
{
	m_numMaterials = numMaterials;

	if (m_pMaterialOverrides)
	{
		GFX_FREE(m_pMaterialOverrides);
		m_pMaterialOverrides = 0;
	}

	size_t sz = sizeof(GfxMaterial*) * numMaterials;

	if (sz)
	{
		m_pMaterialOverrides = (GfxMaterial**)GFX_ALLOCATE(sz);
		memset(m_pMaterialOverrides, 0, sz);
	}
}
//---------------------------------------------------------------------------
bool GfxMeshInstance::clear()
{
	return destroy() && initialize();
}
//---------------------------------------------------------------------------
bool GfxMeshInstance::setTransform(const Transform& xform)
{
	m_transform = xform;
	return true;
}
//---------------------------------------------------------------------------
void GfxMeshInstance::setAABB(const AABB& aabb, bool show)
{
	m_aabb = aabb;
	m_bShowAABB = show ? 1 : 0;
}
//---------------------------------------------------------------------------
void GfxMeshInstance::preRender(GfxRenderer* pRenderer) const
{
	PreRender(pRenderer);
}
//---------------------------------------------------------------------------
void GfxMeshInstance::postRender(GfxRenderer* pRenderer) const
{
	PostRender(pRenderer);
}
