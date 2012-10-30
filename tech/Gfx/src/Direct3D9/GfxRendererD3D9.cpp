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
#include "GfxRendererD3D9.h"
#include "GfxUtil.h"
#include "GfxMesh.h"
#include "GfxSubMesh.h"
#include "GfxMaterial.h"
#include "GfxVertexFormat.h"
#include "GfxVertexDataD3D9.h"
#include "GfxIndexDataD3D9.h"
#include "GfxMeshInstance.h"
#include "GfxTextureStage.h"
#include "GfxTextureD3D9.h"
#include "GfxCamera.h"
#include "GfxLight.h"
#include "GfxShaderLib.h"
#include "shader/GfxShaderD3D9.h"
#include "GfxRenderStats.h"
#include "GfxRenderWindowD3D9.h"
#include "GfxRenderTextureD3D9.h"
#include "GfxViewport.h"
#include "Memory/include/Memory.h"
#include "Memory/include/Allocators.h"
#include "Math/include/Transform.h"
#include "Math/include/Matrix44.h"
#include "Math/include/Quaternion.h"
#include "Math/include/Vector4.h"
#include "Math/include/Vector2.h"
#include "Math/include/MathUtil.h"
#include "Util/include/Environment.h"
#include "Util/include/Hash.h"
#include "Util/include/Logger.h"
#include "Stream/include/FileStream.h"
#include "Resource/include/ResourceManager.h"
#include <assert.h>
#include <new.h>
#include <sstream>
#include <list>
#include <algorithm>

using namespace CoS;
//---------------------------------------------------------------------------
struct CoS::_RenderTargets : public std::list<GfxRenderTarget*> 
{
	DECLARE_GFX_ALLOCATOR();
};
//---------------------------------------------------------------------------
static D3DPRIMITIVETYPE s_d3dPrimTypeLUT[] = {
		//UNKNOWN = 0,
		//POINTLIST,
		//LINELIST,
		//LINESTRIP,
		//TRILIST,
		//TRISTRIP,
		//TRIFAN,
	D3DPT_FORCE_DWORD,
	D3DPT_POINTLIST,
	D3DPT_LINELIST,
	D3DPT_LINESTRIP,
	D3DPT_TRIANGLELIST,
	D3DPT_TRIANGLESTRIP,
	D3DPT_TRIANGLEFAN,
};
//---------------------------------------------------------------------------
GfxRendererD3D9::GfxRendererD3D9(Environment& env) 
: GfxRenderer(env), state(env)
{
	pD3D9 = 0;
	pDevice = 0;
	memset(&paramsPresent, 0, sizeof(paramsPresent));
	pShaderLib = 0;
	m_pAABBMesh = 0;

	m_pRenderTargets = COS_NEW _RenderTargets;
}
//---------------------------------------------------------------------------
GfxRendererD3D9::~GfxRendererD3D9()
{
	delete m_pRenderTargets;
}
//---------------------------------------------------------------------------
bool GfxRendererD3D9::initialize(
	const GfxRenderConfig& cfg, bool createMainWindow)
{
	if (!GfxRenderer::initialize(cfg, createMainWindow))
	{
		return false;
	}

	if (createMainWindow)
		createRenderTarget(m_cfg, FMT_R8G8B8, RT_WINDOW); // inits m_pMainWindow

	// create shader library
	pShaderLib = COS_NEW GfxShaderLib(m_env);
	pShaderLib->initialize();

	// initialize utility meshes (AABB, etc)
	//m_pAABBMesh = GfxUtil::createAABBTemplateMesh(this);

	return true;
}
//---------------------------------------------------------------------------
bool GfxRendererD3D9::destroy()
{
	if (m_env.isOffline)
	{
		return true;
	}

	// destroy shader lib
	if (pShaderLib)
	{
		pShaderLib->destroy();
		delete pShaderLib;
	}

	// shut down D3D9
	if (pDevice)
	{
		pDevice->Release();
	}

	for (_RenderTargets::iterator it = m_pRenderTargets->begin();
		it != m_pRenderTargets->end(); ++it)
	{
		(*it)->destroy();
		delete *it;
	}

	m_pMainWindow = 0;
	m_pRenderTargets->clear();

	if (pD3D9)
	{
		pD3D9->Release();
	}

	return GfxRenderer::destroy();
}
//---------------------------------------------------------------------------
GfxVertexData* GfxRendererD3D9::createVertexData()
{
	GfxVertexDataD3D9* p = COS_NEW GfxVertexDataD3D9;
	return p;
}
//---------------------------------------------------------------------------
void GfxRendererD3D9::trackVertexData(GfxVertexDataD3D9* pData, bool track)
{
	VertexDataSet::iterator it = m_vertexData.find(pData);

	if (track)
	{
		if(it == m_vertexData.end())
			m_vertexData.insert(pData);
	}
	else if (it != m_vertexData.end())
		m_vertexData.erase(it);
}
//---------------------------------------------------------------------------
GfxIndexData* GfxRendererD3D9::createIndexData()
{
	GfxIndexDataD3D9* p = COS_NEW GfxIndexDataD3D9;
	return p;
}
//---------------------------------------------------------------------------
void GfxRendererD3D9::trackIndexData(GfxIndexDataD3D9* pData, bool track)
{
	IndexDataSet::iterator it = m_indexData.find(pData);

	if (track)
	{
		if(it == m_indexData.end())
			m_indexData.insert(pData);
	}
	else if (it != m_indexData.end())
		m_indexData.erase(it);
}
//---------------------------------------------------------------------------
GfxTexture* GfxRendererD3D9::createTexture()
{
	GfxTextureD3D9* p = COS_NEW GfxTextureD3D9;
	return p;
}
//---------------------------------------------------------------------------
void GfxRendererD3D9::trackTexture(GfxTextureD3D9* pTex, bool track)
{
	TextureSet::iterator it = m_textures.find(pTex);

	if (track)
	{
		if(it == m_textures.end())
			m_textures.insert(pTex);
	}
	else if (it != m_textures.end())
		m_textures.erase(it);
}
//---------------------------------------------------------------------------
void GfxRendererD3D9::trackShader(GfxShaderD3D9* pShader, bool track)
{
	ShaderSet::iterator it = m_shaders.find(pShader);

	if (track)
	{
		if(it == m_shaders.end())
			m_shaders.insert(pShader);
	}
	else if (it != m_shaders.end())
		m_shaders.erase(it);
}
//---------------------------------------------------------------------------
void GfxRendererD3D9::releaseDefaultPoolResources()
{
	for (TextureSet::iterator it = m_textures.begin(); 
		it != m_textures.end(); ++it)
	{
		(*it)->releaseD3DResources();
	}
	m_textures.clear();

	// need to clear the sampler state in the state block too
	for (unsigned int i=0; i<GfxRenderState::MAX_SAMPLERS; ++i)
	{
		state.setSampler(i, (GfxTextureStage*)0);
	}

	for (VertexDataSet::iterator it = m_vertexData.begin(); 
		it != m_vertexData.end(); ++it)
	{
		(*it)->releaseD3DResources();
	}
	m_vertexData.clear();

	for (IndexDataSet::iterator it = m_indexData.begin(); 
		it != m_indexData.end(); ++it)
	{
		(*it)->releaseD3DResources();
	}
	m_indexData.clear();

	for (ShaderSet::iterator it = m_shaders.begin(); 
		it != m_shaders.end(); ++it)
	{
		(*it)->releaseD3DResources();
	}
	m_shaders.clear();

	for (_RenderTargets::iterator it = m_pRenderTargets->begin();
		it != m_pRenderTargets->end(); ++it)
	{
		GfxRenderTargetD3D9* pRTD3D9;
		(*it)->queryInterface(GfxRenderTargetD3D9::IID, (void**)&pRTD3D9);
		pRTD3D9->onDeviceReset();
	}

	// reset pertinent cached state data
	state.alphaBlend = false;
	state.setDepthBias(0);
	state.setDepthCheck(true);
	state.setDepthWrite(true);
	state.setDepthFunc(D3DCMP_LESSEQUAL);
	state.setDestBlend(D3DBLEND_ZERO);
	state.setSrcBlend(D3DBLEND_ONE);
}
//---------------------------------------------------------------------------
void GfxRendererD3D9::clearRenderTarget( 
	bool bColor /* = true */, unsigned int clearColor /* = 0 */, 
	bool bDepth /* = true */, float depthValue /* = 1 */, 
	bool bStencil /* = true */, unsigned int stencilValue /* = 0 */ )
{
	if (!m_pCurrentRT)
		return;

	DWORD flags = 0;
	if (bColor)
		flags |= D3DCLEAR_TARGET;
	if (bDepth)
		flags |= D3DCLEAR_ZBUFFER;
	if (bStencil)
		flags |= D3DCLEAR_STENCIL;

	D3DCOLOR col = (D3DCOLOR)clearColor;

	if (flags)
		pDevice->Clear(0, NULL, flags, col, depthValue, stencilValue);
}
//---------------------------------------------------------------------------
bool GfxRendererD3D9::setRenderTarget(GfxRenderTarget* pRT)
{
	assert(pRT);
	if (!pRT)
		return false;

	// because of a probably-wrong design decision, we have to run this 
	// bizarre chain of conversions to get to what we are looking for
	GfxRenderTargetD3D9* pD3D9RT;
	if (!pRT->queryInterface(GfxRenderTargetD3D9::IID, (void**)&pD3D9RT))
		return false;

	// reset the target/window if it needs it (we need to do this
	// before rendering anything because the viewport and back buffer sizes
	// need to match)
	state.setRenderTarget(0, pD3D9RT, pRT->handlePendingResize());
	m_pCurrentRT = pD3D9RT;

	return true;
}
//---------------------------------------------------------------------------
bool GfxRendererD3D9::_swapImpl(GfxRenderStats& /*stats*/)
{
	return true;
}
//---------------------------------------------------------------------------
bool GfxRendererD3D9::setColorWrite(bool bWrite)
{
	bool previous = state.getColorWrite();
	state.setColorWrite(bWrite);
	return previous;
}
//---------------------------------------------------------------------------
void GfxRendererD3D9::beginFrame(
	bool bClearColor,
	unsigned int clearColor,
	bool bClearDepth,
	float depthValue,
	bool bClearStencil,
	unsigned int stencilValue
)
{
	clearRenderTarget(
		bClearColor, clearColor, 
		bClearDepth, depthValue, 
		bClearStencil, stencilValue);

	pDevice->BeginScene();
	state.setCullMode(GfxMaterial::CULL_CCW);
}
//---------------------------------------------------------------------------
void GfxRendererD3D9::endFrame()
{
	assert(m_pCurrentRT);
	if (!m_pCurrentRT)
		return;

	pDevice->EndScene();
	m_pCurrentRT->present();
}
//---------------------------------------------------------------------------
bool GfxRendererD3D9::beginScene(GfxCamera* pCam, GfxViewport* pVP)
{
	setCamera(pCam);
	setupViewport(pVP);
	solidObjects.clear();
	transparentObjects.clear();

	return true;
}
//---------------------------------------------------------------------------
bool GfxRendererD3D9::endScene(bool renderOpaque, bool renderTransparent)
{
	if (renderOpaque)
		renderSolids();
	if (renderTransparent)
		renderTransparents();

	solidObjects.clear();
	transparentObjects.clear();

	return true;
}
//---------------------------------------------------------------------------
bool GfxRendererD3D9::updateAllRenderTargets(GfxRenderStats& /*stats*/)
{
	return true;
}
//---------------------------------------------------------------------------
void GfxRendererD3D9::queueForRendering(const GfxMeshInstance& meshInstance)
{
	// special case that we want to early-out immediately: it is possible
	// for a mesh instance to be set up for instanced rendering, but come in
	// to this method with zero instances set. We want to ignore those. It's 
	// up to the caller to set up their mesh instance properly (either it's
	// doing instanced rendering, or normal rendering).
	if (!meshInstance.m_instanceCount && meshInstance.m_instanceVertexData)
		return;


	// take this opportunity to sort the solids and transparents

	// for efficiency, check the value of the procedural mesh pointer first
	GfxMesh* pMesh = meshInstance.m_pProceduralMesh;

	if (!pMesh)
	{
		// and if the procedural mesh is not set, try the mesh resource
		pMesh = AutoResPtr<GfxMesh>(meshInstance.getMeshHandle());
	}

	// and if we still don't have a mesh (yet), give up
	if (!pMesh)
	{
		return;
	}

	// animated instances get extra post-processing
	//if (meshInstance.getNumMatrices())
	//{
	//	animatedObjects.push_back(meshInstance);
	//}

	bool hasSolid = false, hasTrans = false;
	for (size_t i=0; i<pMesh->getNumSubMeshes(); ++i)
	{
		GfxMaterial* pMtl = pMesh->getSubMesh(i)->getMaterial();

		// prefer material overrides to the material stored in the submesh
		size_t overrides = meshInstance.getMaterialCount();
		if (overrides && i < overrides)
		{
			GfxMaterial* pOverride = meshInstance.getMaterialByIndex(i);
			if (pOverride)
			{
				pMtl = pOverride;
			}
		}

		if (!pMtl)
			continue;

		if (pMtl->isTransparent())
		{
			hasTrans = true;
		}
		else
		{
			hasSolid = true;
		}
	}

	if (hasTrans)
	{
		// init the dist-to-camera value
		Matrix44& v = (Matrix44&)state.view;
		Vector4 camPos(v.m[3][0], v.m[3][1], v.m[3][2], 0);
		float camDist = (meshInstance.getTransform().trans - camPos).getLength2();
		transparentObjects[camDist] = meshInstance;
	}

	if (hasSolid)
	{
		solidObjects.push_back(meshInstance);
	}
}
//---------------------------------------------------------------------------
bool GfxRendererD3D9::setViewport(GfxViewport* pVP)
{
	if (!pVP)
		return false;

	return setupViewport(pVP);
}
//---------------------------------------------------------------------------
bool GfxRendererD3D9::setCamera(GfxCamera* pCam)
{
	if (!pCam)
	{
		return false;
	}

	// set up the camera to use for this VP
	state.setViewMatrix(pCam->getViewMatrix());
	state.setProjectionMatrix(pCam->getProjectionMatrix());
	state.setCameraDepth(Vector4(pCam->getFarClip() - pCam->getNearClip(), 0, 0, 0));

	return true;
}
//---------------------------------------------------------------------------
bool GfxRendererD3D9::setScreenSpaceMatrices(GfxViewport* pVP)
{
	// set up projection matrix in such a way that we can supply pixel
	// dimensions to screen-space objects
	const Vector2& size = pVP->getSize();
	const Vector2& pos = pVP->getPosition();

	float oow = 1 / size.x;
	float ooh = 1 / size.y;

	// set up identity view and projection matrices
	Matrix44 view(Matrix44::IDENTITY);
	view.m[1][1] = -1;
	state.setViewMatrix(view);

	Matrix44 proj(Matrix44::IDENTITY);
	proj.m[0][0] = 2 * oow;
	proj.m[1][1] = 2 * ooh;
	proj.m[3][0] = pos.x * oow - 1;
	proj.m[3][1] = pos.y * ooh + 1;
	proj.m[2][3] = 1;
	
	state.setProjectionMatrix(proj);

	return true;
}
//---------------------------------------------------------------------------
bool GfxRendererD3D9::setupViewport(GfxViewport* pVP)
{
	assert(pVP);
	if (!pVP)
	{
		return false;
	}

	Vector2 pos(pVP->getPosition());
	Vector2 size(pVP->getSize());

	D3DVIEWPORT9 vp;
	vp.X = (DWORD)pos.x;
	vp.Y = (DWORD)pos.y;
	vp.Width = (DWORD)size.x;
	vp.Height = (DWORD)size.y;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;

	pDevice->SetViewport(&vp);
	return true;
}

//---------------------------------------------------------------------------
void GfxRendererD3D9::renderSolids()
{
	for(VisibleObjects::iterator it = solidObjects.begin();
		it != solidObjects.end(); ++it)
	{
		if (it->getMeshHandle() == INVALID_RESOURCE_HANDLE &&
			!it->m_pProceduralMesh)
		{
			continue;
		}

		GfxMeshInstance& rInst = *it;
		Matrix44 xform;

		MathUtil::transformToMatrix44(rInst.getTransform(), xform);
		state.setWorldMatrix(xform);

		// recalc WVP
		//Matrix44 out, t;
		//mul(t, xform, (Matrix44&)state.view);
		//mul(out, t, (Matrix44&)state.proj);
		//out.transpose(t);
		//state.setWVP(t);

		renderMeshInstance(rInst, false);
	}
}
//---------------------------------------------------------------------------
void GfxRendererD3D9::renderTransparents()
{
	// turn off depth write and turn on depth check
	state.setSrcBlend(D3DBLEND_SRCALPHA);
	state.setDestBlend(D3DBLEND_INVSRCALPHA);

	for(DistanceSortedObjects::reverse_iterator it = transparentObjects.rbegin();
		it != transparentObjects.rend(); ++it)
	{
		GfxMeshInstance& rInst = it->second;
		if (rInst.getMeshHandle() == INVALID_RESOURCE_HANDLE &&
			!rInst.m_pProceduralMesh)
		{
			continue;
		}

		Matrix44 xform;
		MathUtil::transformToMatrix44(rInst.getTransform(), xform);
		state.setWorldMatrix(xform);

		// recalc WVP
		//Matrix44 out, t;
		//mul(t, xform, (Matrix44&)state.view);
		//mul(out, t, (Matrix44&)state.proj);
		//out.transpose(t);
		//state.setWVP(t);

		renderMeshInstance(rInst, true);
	}
}
//---------------------------------------------------------------------------
static size_t getPrimitiveCount(
	PrimitiveType type, 
	size_t iCount=0,
	size_t vCount=0
	)
{
	switch (type)
	{
	case TRILIST:
		if (iCount)
			return iCount / 3;
		else
			return vCount / 3;
		break;

	case TRISTRIP:
	case TRIFAN:
		if (iCount)
			return iCount - 2;
		else
			return vCount - 2;
		break;

	case POINTLIST:
		if (iCount)
			return iCount;
		else
			return vCount;
		break;

	case LINELIST:
		if (iCount)
			return iCount / 2;
		else
			return vCount / 2;
		break;

	case LINESTRIP:
		if (iCount)
			return iCount - 1;
		else
			return vCount - 1;
		break;
	}

	return 0;
}
//---------------------------------------------------------------------------
bool GfxRendererD3D9::renderSubMesh(
	const GfxMeshInstance& rInst,
	GfxSubMesh* pSubMesh,
	size_t i, // submesh index 
	bool trans
	)
{
	GfxMaterial* pMtl = pSubMesh->getMaterial();

	Matrix44 world;
	MathUtil::transformToMatrix44(rInst.getTransform(), world);

	// prefer material overrides to the material stored in the submesh
	size_t overrides = rInst.getMaterialCount();
	if (overrides && i < overrides)
	{
		GfxMaterial* pOverride = rInst.getMaterialByIndex(i);
		if (pOverride)
		{
			pMtl = pOverride;
		}
	}

	// filter solids and transparents...
	if (pMtl->isTransparent() != trans)
	{
		return true;
	}

	// set up light parameters 
	const GfxLight** pLights = rInst.m_pLights;
	Vector4 camDepth(Vector4::ZERO);
	float* pF = (float*)&camDepth;
	size_t numTextures = pMtl->getNumTextureStages();

	if (pLights)
	{
		for (size_t i=0; i<rInst.m_numLights; ++i)
		{
			const GfxLight* pLight = pLights[i];
			if (pLight)
			{
				GfxLight::Type type = pLight->getType();
				if (type == GfxLight::LT_POINT || type == GfxLight::LT_SPOT)
				{
					Vector4 pos(pLight->getPosition());
					pos.w = 1; // flag for use in the shader
					state.setLightPos(pos, i);
				}
				if (type == GfxLight::LT_DIRECTIONAL || type == GfxLight::LT_SPOT)
				{
					Vector4 dir(-pLight->getDirection());
					dir.w = 1; // flag for use in the shader
					state.setLightDir(dir, i);
				}

				Vector4 col;
				pLight->getColor(col.x, col.y, col.z);
				col.w = (GfxLight::LT_SPOT == type) ? 1.f : 0.f; // indicate whether or not light is a spot
				state.setLightColor(col, i);

				const GfxCamera* pShadowCam = pLight->getShadowCamera();
				if (pShadowCam)
				{
					state.setLightViewProjMatrix(pShadowCam->getViewProjMatrix(), i);
					if (i < 4)
						pF[i] = pShadowCam->getFarClip() - pShadowCam->getNearClip();

					// set the shadow texture if this is a shadow-casting light and the 
					// object is a shadow-receiving one
					GfxRenderTarget* pRTex = const_cast<GfxRenderTarget*>(pLight->getShadowTexture());
					GfxRenderTexture* pShadowTex;
					pRTex->queryInterface(GfxRenderTexture::IID, (void**)&pShadowTex);
					if (rInst.m_bReceiveShadows && pShadowTex)
					{
						// tack onto the end of the material's sampler list
						GfxTexture* pTex = AutoResPtr<GfxTexture>(pShadowTex->getTextureHandle());
						state.setSampler(numTextures + i, pTex);
					}
				}
			}
			else
			{
				state.setLightColor(Vector4::ZERO, i);
			}
		}

		state.setCameraDepth(camDepth);
	}

	if (m_renderMode == RENDER_DEFAULT)
	{
		state.setCullMode(pMtl->getCullMode());
		state.setDepthCheck(pMtl->getDepthCheck());
		state.setDepthWrite(pMtl->getDepthWrite());
		state.setFillMode(pMtl->getFillMode());
		state.setDepthBias(pMtl->getDepthBias());
		state.enableAlphaBlend(pMtl->isAlphaBlended());
	}

	const Matrix44* pPalette = rInst.m_pMatrixPalette;
	for (size_t i=0; i<rInst.m_numMatrices; ++i)
	{
		Matrix44* pBP = 0;
		if (pSubMesh->getBindPoses().size())
			pBP = pSubMesh->getBindPoses()[i];

		if (pBP)
		{
			Matrix44 out, out2;
			mul(out2, pPalette[i], world);
			mul(out, *pBP, out2);
			state.setMatrixPaletteEntry(out, i);
		}
		else
		{
			state.setMatrixPaletteEntry(pPalette[i], i);
		}
	}

	// ...and only render the ones requested
	setupShader(*pSubMesh, *pMtl);
	GfxShaderD3D9* pShader = pShaderLib->createShader(pSubMesh, pMtl, rInst.m_pShader);
	if (!pShader)
		return true;

	GfxShaderD3D9::ShaderVariant sv = GfxShaderD3D9::SV_DEFAULT;
	if (m_renderMode == RENDER_SHADOW_CASTERS)
		sv = GfxShaderD3D9::SV_SHADOWCASTER;
	else
	{
		if (rInst.m_bReceiveShadows)
			sv = GfxShaderD3D9::SV_SHADOWRECEIVER;
	}

	if (!pShader->setTechnique(sv))
	{
		if (sv == GfxShaderD3D9::SV_SHADOWRECEIVER)
		{
			// try again with a default technique
			if (!pShader->setTechnique(GfxShaderD3D9::SV_DEFAULT))
				return true; // give up and move on
		}
	}

	ID3DXEffect* pFX = pShader->getFX();
	if (!pFX)
		return true; // don't render it because we can't...

	UINT passes;
	pFX->Begin(&passes, D3DXFX_DONOTSAVESTATE);
	pFX->BeginPass(0);
	pShader->setState(&state, rInst.m_pShaderConstants, rInst.m_numMatrices);
	pFX->CommitChanges();
	
	if (rInst.m_instanceCount)
	{
		if (!renderInstancedGeometry(rInst, pSubMesh))
			return false;
	}
	else
	{
		if (!renderNormalGeometry(pSubMesh))
			return false;
	}

	pFX->EndPass();
	pFX->End();

	// clear out any shadow textures we may have set up -- D3D doesn't seem to 
	// much like rendering to textures that are still bound
	size_t numStages = pMtl->getNumTextureStages();
	size_t maxStages = pMtl->getMaxNumTextureStages();
	for (size_t i=numStages; i<maxStages; ++i)
	{
		state.setSampler(i, (GfxTexture*)0);
	}

	return true;
}
//---------------------------------------------------------------------------
bool GfxRendererD3D9::renderNormalGeometry(GfxSubMesh *pSubMesh)
{
	// TODO: support offsets into vertex/index buffer
	size_t vertOffset = 0;
	size_t indexOffset = 0;

	PrimitiveType primType = UNKNOWN;
	size_t vertCt = 0;
	size_t primCount = 0;

	// set vertex declaration
	GfxVertexFormat& fmt = pSubMesh->getVertexFormat();
	IDirect3DVertexDeclaration9* pDecl = 
		(IDirect3DVertexDeclaration9*)fmt.getImpl();

	if (!pDecl)
	{ 
		fmt.fix(m_env);
	}

	pDecl = (IDirect3DVertexDeclaration9*)fmt.getImpl();
	pDevice->SetVertexDeclaration(pDecl);

	// set vertex stream(s)
	for (size_t j=0; j<pSubMesh->getNumVertexData(); ++j)
	{
		GfxVertexDataD3D9* pVData = 
			static_cast<GfxVertexDataD3D9*>(pSubMesh->getVertexData(j));

		if (pVData)
		{
			IDirect3DVertexBuffer9* pD3Dbuf = pVData->getD3D9VertexBuffer();

			pDevice->SetStreamSource(
				(UINT)j, 
				pD3Dbuf, 
				0, 
				(UINT)pVData->getVertexSize());

			// all vertex data in the same submesh should be the 
			// same prim type
			primType = pSubMesh->getPrimitiveType();
			vertCt = pVData->getVertexCount();
		}
	}

	// set index stream
	GfxIndexDataD3D9* pIData = static_cast<GfxIndexDataD3D9*>(pSubMesh->getIndexData());
	if (pIData)
	{
		IDirect3DIndexBuffer9* pIB = pIData->getD3D9IndexBuffer();
		if (!pIB)
		{
			pIData->delayInit();
		}

		pIB = pIData->getD3D9IndexBuffer();
		pDevice->SetIndices(pIB);

		primCount = getPrimitiveCount(primType, 
			pIData->getIndexCount() - indexOffset);

		// draw the indexed primitive
		if (primType == UNKNOWN)
		{
			return false;
		}

		if (FAILED(pDevice->DrawIndexedPrimitive(
			s_d3dPrimTypeLUT[primType],
			(INT)vertOffset,
			0, // min vertex index relative to vertOffset
			(UINT)vertCt,
			(UINT)indexOffset,
			(UINT)primCount)))
		{
			return false;
		}
	}
	else // non-indexed primitive type
	{
		primCount = getPrimitiveCount(primType, 0, vertCt);

		// draw the indexed primitive
		if (primType == UNKNOWN)
		{
			return false;
		}

		if (FAILED(pDevice->DrawPrimitive(
			s_d3dPrimTypeLUT[primType],
			(INT)vertOffset,
			(UINT)primCount)))
		{
			return false;
		}
	}

	// update render stats
	//++m_pCurrentRenderStats->numBatches;
	//m_pCurrentRenderStats->numTris += primCount;
	//m_pCurrentRenderStats->numVerts += vertCt - vertOffset;

	return true;
}
//---------------------------------------------------------------------------
bool GfxRendererD3D9::renderInstancedGeometry(
	const GfxMeshInstance& rInst, GfxSubMesh* pSubMesh)
{
	// TODO: support offsets into vertex/index buffer
	size_t vertOffset = 0;
	size_t indexOffset = 0;

	PrimitiveType primType = UNKNOWN;
	size_t vertCt = 0;
	size_t primCount = 0;

	// set vertex declaration
	GfxVertexFormat& fmt = pSubMesh->getVertexFormat();
	IDirect3DVertexDeclaration9* pDecl = 
		(IDirect3DVertexDeclaration9*)fmt.getImpl();

	if (!pDecl)
	{ 
		fmt.fix(m_env);
	}

	pDecl = (IDirect3DVertexDeclaration9*)fmt.getImpl();
	pDevice->SetVertexDeclaration(pDecl);

	// set vertex stream(s)
	size_t j=0;
	for (; j<pSubMesh->getNumVertexData(); ++j)
	{
		GfxVertexDataD3D9* pVData = 
			static_cast<GfxVertexDataD3D9*>(pSubMesh->getVertexData(j));

		if (pVData)
		{
			IDirect3DVertexBuffer9* pD3Dbuf = pVData->getD3D9VertexBuffer();

			pDevice->SetStreamSource(
				(UINT)j, 
				pD3Dbuf, 
				0, 
				(UINT)pVData->getVertexSize());

			// set this stream as the geometry stream
			pDevice->SetStreamSourceFreq(
				j, 
				D3DSTREAMSOURCE_INDEXEDDATA | rInst.m_instanceCount);

			// all vertex data in the same submesh should be the 
			// same prim type
			primType = pSubMesh->getPrimitiveType();
			vertCt = pVData->getVertexCount();
		}
	}

	// set instance data in the next stream (usually 1)

	// set stream 1 as the instance data stream
	pDevice->SetStreamSourceFreq(
		(UINT)j,
		(UINT)D3DSTREAMSOURCE_INSTANCEDATA | 1
		);

	GfxVertexDataD3D9* pInstVData = 
		static_cast<GfxVertexDataD3D9*>(rInst.m_instanceVertexData);

	if (pInstVData)
	{
		IDirect3DVertexBuffer9* pD3DInstData = pInstVData->getD3D9VertexBuffer();

		pDevice->SetStreamSource(
			j,
			pD3DInstData,
			0,
			sizeof(D3DXMATRIX));
	}

	// set index stream
	GfxIndexDataD3D9* pIData = static_cast<GfxIndexDataD3D9*>(pSubMesh->getIndexData());
	if (pIData)
	{
		IDirect3DIndexBuffer9* pIB = pIData->getD3D9IndexBuffer();
		if (!pIB)
		{
			pIData->delayInit();
		}

		pIB = pIData->getD3D9IndexBuffer();
		pDevice->SetIndices(pIB);

		primCount = getPrimitiveCount(primType, 
			pIData->getIndexCount() - indexOffset);

		// draw the indexed primitive
		if (primType == UNKNOWN)
		{
			return false;
		}

		if (FAILED(pDevice->DrawIndexedPrimitive(
			s_d3dPrimTypeLUT[primType],
			(INT)vertOffset,
			0, // min vertex index relative to vertOffset
			(UINT)vertCt,
			(UINT)indexOffset,
			(UINT)primCount)))
		{
			return false;
		}
	}

	// reset the freq values back to the defaults
	for (size_t k=0; k<=j; ++k)
	{
		pDevice->SetStreamSourceFreq(k, 1);
	}

	// update render stats
	//++m_pCurrentRenderStats->numBatches;
	//m_pCurrentRenderStats->numTris += primCount;
	//m_pCurrentRenderStats->numVerts += vertCt - vertOffset;

	return true;
}
//---------------------------------------------------------------------------
bool GfxRendererD3D9::renderMeshInstance(
	const GfxMeshInstance& rInst, bool trans)
{
	if (rInst.getMeshHandle() == INVALID_RESOURCE_HANDLE &&
		!rInst.m_pProceduralMesh)
	{
		return false;
	}

	GfxMesh* pMesh = rInst.m_pProceduralMesh;
	if (!pMesh)
	{
		pMesh = AutoResPtr<GfxMesh>(rInst.getMeshHandle());

		if (!pMesh)
			return false;
	}

	if (!pMesh->getNumSubMeshes())
	{
		return false;
	}

	// call any existing hooks on the render of this instance
	//rInst.preRender(this);

	GfxMesh& rMesh = *pMesh;
	// TODO: support shared vertex data
	for (size_t i=0; i<rMesh.getNumSubMeshes(); ++i)
	{
		GfxSubMesh* pSubMesh = rMesh.getSubMesh(i);
		if (!renderSubMesh(rInst, pSubMesh, i, trans))
			return false;
	}

	// display the mesh AABB if requested
	if (rInst.showAABB() && m_pAABBMesh)
	{
		// create transform from the AABB
		Matrix44 world(Matrix44::IDENTITY);

		const AABB& aabb = rInst.getAABB();
		const Vector4& center = aabb.center;

		world.m[3][0] = center.x;
		world.m[3][1] = center.y;
		world.m[3][2] = center.z;

		// recalc WVP
		Matrix44 out, t;
		mul(t, world, (Matrix44&)state.view);
		mul(out, t, (Matrix44&)state.proj);
		out.transpose(t);
		state.setWVP(t);

		if (!renderSubMesh(rInst, m_pAABBMesh->getSubMesh(0)))
			return false;
	}

	//rInst.postRender(this);

	return true;
}
//---------------------------------------------------------------------------
void GfxRendererD3D9::setAmbientLight(const Vector4& ambient)
{
	state.setWorldAmbient(ambient);
}
//---------------------------------------------------------------------------
GfxShader* GfxRendererD3D9::getShader(
	GfxSubMesh* pSubMesh, 
	GfxMaterial* pMtl,
	const char* pFilename)
{
	unsigned __int64 hash;
	if (pShaderLib)
		return pShaderLib->findShader(pSubMesh, pMtl, pFilename, hash);

	return 0;
}
//---------------------------------------------------------------------------
void GfxRendererD3D9::logD3DError(HRESULT hr)
{
	std::stringstream ss;
	ss << DXGetErrorString(hr) << ": ";
	ss << DXGetErrorDescription(hr);
	m_env.pLogger->logMessage(ss.str().c_str());
}
//---------------------------------------------------------------------------
bool GfxRendererD3D9::setupShader(GfxSubMesh& rSubMesh, GfxMaterial& mtl)
{
	rSubMesh;
	// these will return the shader if it already exists
	//GfxShaderD3D9* pShader = pShaderLib->createShader(&rSubMesh, &mtl);

	// lighting
	if (!mtl.isLit())
	{
		if (state.lighting)
		{
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
			state.lighting = false;
		}
	}
	else
	{
		if (!state.lighting)
		{
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
			state.lighting = true;
		}
	}

	// FF material -- for testing
	Vector4 dc, ac, sc, ec;
	GfxUtil::unpackColor(mtl.getDiffuse(), dc);
	GfxUtil::unpackColor(mtl.getAmbient(), ac);
	GfxUtil::unpackColor(mtl.getSpecular(), sc);
	GfxUtil::unpackColor(mtl.getEmissive(), ec);

	// update the local state
	state.setMaterialDiffuse(dc);
	state.setMaterialAmbient(ac);
	state.setMaterialSpecular(sc);
	state.setMaterialEmissive(ec);

	// texture samplers and states
	for (size_t i=0; i<mtl.getNumTextureStages(); ++i)
	{
		GfxTextureStage* pStage = mtl.getTextureStage(i);
		if (pStage)
		{
			HResource hTex = pStage->getTextureHandle();
			
			if (hTex != INVALID_RESOURCE_HANDLE)
			{
				state.setSampler((unsigned int)i, pStage);
			}
		}
	}

	return true;
}
//---------------------------------------------------------------------------
GfxRenderTarget* GfxRendererD3D9::createRenderTarget(
	GfxRenderConfig& cfg,
	SurfaceFormat fmt,
	RenderTargetType type)
{
	if (type == RT_WINDOW)
	{
		GfxRenderWindowD3D9* pWin = COS_NEW GfxRenderWindowD3D9;

		if (!pWin->initialize(cfg, fmt))
		{
			delete pWin;
			return false;
		}

		// first one is the "main" window for the device
		if (!m_pMainWindow)
		{
			m_pMainWindow = pWin;
			pWin->setAsMainWindow();
		}

		m_pRenderTargets->push_back(pWin);
		return pWin;
	}

	if (type == RT_TEXTURE)
	{
		GfxRenderTextureD3D9* pTex = COS_NEW GfxRenderTextureD3D9;

		if (!pTex->initialize(cfg, fmt))
		{
			delete pTex;
			return false;
		}

		m_pRenderTargets->push_back(pTex);
		return pTex;
	}

	return 0; // todo: render texture targets
}
//---------------------------------------------------------------------------
void GfxRendererD3D9::destroyRenderTarget(GfxRenderTarget* pRT)
{
	if (pRT)
	{
		for (_RenderTargets::iterator it = m_pRenderTargets->begin();
			it != m_pRenderTargets->end(); ++it)
		{
			if (*it == pRT)
			{
				m_pRenderTargets->erase(it);
				break;
			}
		}

		pRT->destroy();
		delete pRT;
	}
}
//---------------------------------------------------------------------------
bool GfxRendererD3D9::writeVertexFloatRegister(
	float* pFloats, int startReg, int numFloatRegisters)
{
	if (!pFloats)
		return false;

	memcpy(&state.vfConst[startReg][0], 
		pFloats, numFloatRegisters * sizeof(float) * 4);

	// update the changed data
	HRESULT hr = pDevice->SetVertexShaderConstantF(
		startReg,
		&state.vfConst[startReg][0],
		numFloatRegisters);

	if (FAILED(hr))
	{
		logD3DError(hr);
	}

	return true;
}
//---------------------------------------------------------------------------
bool GfxRendererD3D9::writeFragmentFloatRegister(
	float* pFloats, int startReg, int numFloatRegisters)
{
	if (!pFloats)
		return false;

	memcpy(&state.pfConst[startReg][0], 
		pFloats, numFloatRegisters * sizeof(float) * 4);

	// update the changed data
	HRESULT hr = pDevice->SetPixelShaderConstantF(
		startReg,
		&state.pfConst[startReg][0],
		numFloatRegisters);

	if (FAILED(hr))
	{
		logD3DError(hr);
	}

	return true;
}
