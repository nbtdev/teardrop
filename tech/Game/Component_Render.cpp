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

#include "Component_Render.h"
#include "Zone.h"
#include "Scene.h"
#include "ZoneObject.h"
#include "DynamicLight.h"
#include "Gfx/GfxMesh.h"
#include "Gfx/GfxSubMesh.h"
#include "Gfx/GfxMaterial.h"
#include "Gfx/GfxRenderer.h"
#include "Gfx/IMeshInstanceProvider.h"
#include "Util/Hash.h"
#include "Util/FourCC.h"
#include "Util/Environment.h"
#include "Resource/ResourceManager.h"
#include "Resource/ResourceHandle.h"
#include <algorithm>

using namespace CoS;
using namespace Reflection;
//---------------------------------------------------------------------------
COS_CLASS_IMPL(RenderComponent);
//---------------------------------------------------------------------------
RenderComponent::RenderComponent()
{
	m_pHost = 0;
	m_bNeedLightsUpdated = false;
	m_meshInst.m_pShaderConstants = &m_constants;
}
//---------------------------------------------------------------------------
RenderComponent::~RenderComponent()
{
}
//---------------------------------------------------------------------------
void RenderComponent::onInstanceCreated()
{
	setServerComponent(false); // render not needed on a server
}
//---------------------------------------------------------------------------
void RenderComponent::recalculateLighting()
{
	// determine if this mesh instance is lit (by examining the mesh 
	// materials and overrides
	setLit(false);
	GfxMesh* pMesh = AutoResPtr<GfxMesh>(m_meshInst.getMeshHandle());
	if (pMesh)
	{
		for (size_t i=0; i<pMesh->getNumSubMeshes(); ++i)
		{
			const GfxMaterial* pMtl = m_meshInst.getMaterialByIndex(i);
			if (!pMtl)
				pMtl = pMesh->getSubMesh(i)->getMaterial();

			getLit() |= pMtl->isLit();
		}
	}

	m_bNeedLightsUpdated = getLit();
}
//---------------------------------------------------------------------------
void RenderComponent::setMeshInstance(const GfxMeshInstance& inst)
{
	m_meshInst = inst;
	m_meshInst.m_pShaderConstants = &m_constants;
	m_meshInst.m_pShader = (const char*)getShaderName();
	m_meshInst.m_bCastShadows = getShadowCaster();
	m_meshInst.m_bReceiveShadows = getShadowReceiver();
	recalculateLighting();
}
//---------------------------------------------------------------------------
void RenderComponent::updateTransform(const Transform& xform)
{
	m_meshInst.setTransform(xform);

	if (m_pLights.isNull())
		recalculateLighting();

	// we now need to update lights that may affect this component
	m_bNeedLightsUpdated = getLit();
}
//---------------------------------------------------------------------------
bool RenderComponent::initialize()
{
	if (!Environment::get().pRenderer)
		return true; // early out if we are not rendering

	m_meshInst.setTransform(m_pHost->getTransformWS());
	m_meshInst.m_pShader = (const char*)getShaderName();
	m_meshInst.m_bCastShadows = getShadowCaster();
	m_meshInst.m_bReceiveShadows = getShadowReceiver();

	if (getMeshName() != "(undefined)")
	{
		String name(m_assetPath);
		name += "/";
		name += getMeshName();

		m_meshInst.setMeshHandle(
			Environment::get().pResourceMgr->acquire(
			GfxMesh::RESOURCE_TYPE, name));

		recalculateLighting();
	}
	return true;
}
//---------------------------------------------------------------------------
bool RenderComponent::destroy()
{
	Environment::get().pResourceMgr->release(m_meshInst.getMeshHandle());
	return true;
}
//---------------------------------------------------------------------------
bool RenderComponent::update(float /*deltaT*/)
{
	// get the current transform from our host
	updateTransform(m_pHost->getTransformWS());
	return true;
}
//---------------------------------------------------------------------------
void RenderComponent::updateMatrixPalette(const Matrix44* pPalette, size_t sz)
{
	if (pPalette && sz)
	{
		m_meshInst.m_pMatrixPalette = pPalette;
		m_meshInst.m_numMatrices = sz;
	}
}
//---------------------------------------------------------------------------
static bool lightFilter(ZoneObject* pObj)
{
	if (pObj->getDerivedClassDef()->isA(DynamicLight::getClassDef()))
	{
		return true;
	}

	return false;
}
//---------------------------------------------------------------------------
void RenderComponent::queueForRendering(GfxRenderer* pRenderer)
{
	// if we are disabled, don't queue any renderables
	if (!getEnabled())
		return;

	// queue the one we have 
	pRenderer->queueForRendering(m_meshInst);

	// and then the ones from registered providers
	for (MeshInstanceProviders::iterator it = m_meshInstProviders.begin();
		it != m_meshInstProviders.end(); ++it)
	{
		(*it)->queueForRendering(pRenderer);
	}
}
//---------------------------------------------------------------------------
void RenderComponent::addMeshInstanceProvider(IMeshInstanceProvider* pProvider)
{
	m_meshInstProviders.push_back(pProvider);
}
//---------------------------------------------------------------------------
void RenderComponent::removeMeshInstanceProvider(IMeshInstanceProvider* pProvider)
{
	MeshInstanceProviders::iterator it = std::find(
		m_meshInstProviders.begin(), m_meshInstProviders.end(), pProvider);

	if (it != m_meshInstProviders.end())
		m_meshInstProviders.erase(it);
}
//---------------------------------------------------------------------------
void RenderComponent::updateLightList(Scene* pScene)
{
	// this is not something we want to do any more than necessary
	if (!m_bNeedLightsUpdated)
		return;

	m_bNeedLightsUpdated = false;

	// note also that going directly to the zone means that we cannot be
	// lit by lights in other zones (even if they are closer than ones in 
	// this zone) -- issue to be addressed at a later date
	ZoneObjects objects;
	if (pScene->findObjectsWithinRadius(
		objects,
		m_pHost->getTransformWS().trans, 
		1000, // this is a material property and will have to be addressed later
		lightFilter))
	{
		// we need to sort these in order of increasing distance from us, and also 
		// put directional lights at the front of the list
		m_pLights = LightListPtr(new LightList(objects.size()));
		for (size_t i=0; i<objects.size(); ++i)
		{
			DynamicLight* pLight = static_cast<DynamicLight*>(objects[i]);
			(*m_pLights)[i] = static_cast<GfxLight*>(pLight);
		}

		m_meshInst.m_pLights = (const GfxLight**)&((*m_pLights.get())[0]);
		m_meshInst.m_numLights = m_pLights->size();
	}
}
