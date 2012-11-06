/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "UIFlashElement.h"
#include "UIFlashInstance.h"
#include "UIRenderer.h"
#include "Gfx/GfxMesh.h"
#include "Gfx/GfxSubMesh.h"
#include "Gfx/GfxVertexData.h"
#include "Gfx/GfxVertexFormat.h"
#include "Gfx/GfxMaterial.h"
#include "Gfx/GfxUtil.h"
#include "Gfx/GfxTextureStage.h"
#include "Gfx/GfxTexture.h"
#include "Resource/ResourceManager.h"
#include "Util/Environment.h"
#include "Memory/Allocators.h"
#include <assert.h>

using namespace Teardrop;
using namespace UI;
//---------------------------------------------------------------------------
FlashElement::FlashElement()
{
	m_pMtl = 0;
	m_pFlash = 0;
	m_pNotify = 0;
	m_pData = 0;
}
//---------------------------------------------------------------------------
FlashElement::~FlashElement()
{
	destroy();
}
//---------------------------------------------------------------------------
struct Vertex
{
	float pos[3];
	//unsigned int color;
	float uv[2];
};
static Vertex s_quadVerts[] = 
{
	/*3D position*/	/*color*/		 /*texcoord*/
	{0, 0, 0,		/*0xFFFFFFFF,*/	 0, 0},
	{1, 0, 0,		/*0xFF000000,*/	 1, 0},
	{0, 1, 0,		/*0xFF000000,*/	 0, 1},
	{1, 1, 0,		/*0xFF000000,*/	 1, 1},
};
//---------------------------------------------------------------------------
bool FlashElement::initialize(
	 const char* resourceName, HResource hTexture, const ElementParams& params)
{
	m_size = Vector2(params.w, params.h);
	unsigned int width = (unsigned int)m_size.x;
	unsigned int height = (unsigned int)m_size.y;

	// get an instance of a Flash player interface object
	m_pFlash = TD_NEW FlashInstance;
	m_pFlash->setSize(width, height);
	m_pFlash->initialize(params.resourceName, this);

	// get a GfxMesh from the resource manager
	HResource hQuad = Environment::get().pResourceMgr->acquire(GfxMesh::RESOURCE_TYPE);

	// assumption: this is immediately available when we create one with no name
	GfxMesh* pMesh = AutoResPtr<GfxMesh>(hQuad);
	assert(pMesh);

	if (!pMesh)
		return false;

	GfxSubMesh* pSM = pMesh->createSubMesh();

	GfxVertexFormat fmt;
	GfxVertexFormat::Element elem;
	elem.semantic = POSITION;
	elem.type = FLOAT3;
	fmt.addElement(elem);

	//elem.semantic = COLOR;
	//elem.offset = GfxUtil::getSizeOf((VertexElementType)elem.type);
	//elem.type = VET_COLOR;
	//fmt.addElement(elem);

	elem.semantic = TEXCOORD;
	elem.offset = GfxUtil::getSizeOf((VertexElementType)elem.type);
	elem.type = FLOAT2;
	fmt.addElement(elem);
	pSM->setVertexFormat(Environment::get(), fmt);

	size_t idx;
	GfxVertexData* pVD = pSM->createVertexData(
		idx,
		Environment::get(),
		fmt.getVertexSize(),
		4,
		(GfxVertexData::CreationFlags)
			(GfxVertexData::STATIC | GfxVertexData::WRITE_ONLY),
		s_quadVerts);

	pSM->setPrimitiveType(TRISTRIP);

	// create the material that goes along with this thing
	m_pMtl = TD_NEW GfxMaterial;
	m_pMtl->initialize();
	m_pMtl->setDiffuse(0xffffffff);
	m_pMtl->setDepthWrite(false);
	m_pMtl->setAlphaBlended(true);

	// we need to create a texture to hold the rendered Flash movie blits
	GfxTextureStage* pStage = m_pMtl->addTextureStage();
	pStage->setFilter(GfxTextureStage::NEAREST, GfxTextureStage::NEAREST, GfxTextureStage::NEAREST);
	pStage->setTextureAddressing(GfxTextureStage::CLAMP, GfxTextureStage::CLAMP, GfxTextureStage::CLAMP);
	pStage->setEnabled(true);

	// create a texture of sufficient size and depth (RGBA)
	hTexture = Environment::get().pResourceMgr->acquire(GfxTexture::RESOURCE_TYPE);

	// assumption: this is immediately available when we create one with no name
	GfxTexture* pTex = AutoResPtr<GfxTexture>(hTexture);
	assert(pTex);

	// get movie dimensions from the flash instance
	unsigned int depth = 4;
	unsigned int buflen = width * height * depth;

	// lifetime will be owned by the GfxTexture; we are creating it 
	// here to have access to the data without having to lock the texture
	// every time the Flash movie updates something
	m_pData = TD_ALLOCATE_ALIGNED(DEFAULT, buflen, 16);

	pTex->initialize(
		width, 
		height,
		depth,
		GfxTexture::UNKNOWN,
		GfxTexture::TEX2D,
		1,
		1, // ATI hardware doesn't seem to like non-POW2 with multiple mips 
		   // (not that we need them for screen-space textures)
		m_pData,
		buflen);

	pStage->setTexture(0, hTexture);

	pSM->setMaterial(m_pMtl);

	m_inst.setMeshHandle(hQuad);

	return true;
}
//---------------------------------------------------------------------------
void FlashElement::destroy()
{
	// release our instance of a Flash player interface object
	if (m_pFlash)
		m_pFlash->destroy();
	delete m_pFlash;
	m_pFlash = 0;

	Environment::get().pResourceMgr->release(m_inst.getMeshHandle());
	
	if (m_pMtl)
		m_pMtl->destroy();

	delete m_pMtl;
}
//---------------------------------------------------------------------------
bool FlashElement::render(Renderer* pRenderer)
{
	if (m_bVisible)
	{
		pRenderer->render(this);
	}

	return true;
}
//---------------------------------------------------------------------------
bool FlashElement::update(float deltaT)
{
	if (!m_pFlash)
		return false;

	if (!m_bVisible)
		return true;

	if (m_pFlash->isDirty())
	{
		// then need to upload the texture data
		HResource hTex = m_pMtl->getTextureStage(0)->getTextureHandle();
		GfxTexture* pTex = AutoResPtr<GfxTexture>(hTex);
		if (pTex)
		{
			int pitch;
			void *pData = pTex->lock(pitch, true);
			m_pFlash->update(pData, pitch, deltaT);
			pTex->unlock();
		}
	}

	return true;
}
//---------------------------------------------------------------------------
void FlashElement::onFSCommand(const char* cmd, const char* args)
{
	if (m_pNotify)
		m_pNotify->onCommand(cmd, args);
}
//---------------------------------------------------------------------------
void FlashElement::onFlashCall(const char* cmd)
{
	if (m_pNotify)
		m_pNotify->onCommand(cmd);
}
//---------------------------------------------------------------------------
bool FlashElement::injectMouseMove(int x, int y)
{
	int nx = (int)m_positionCache.x;
	int ny = (int)m_positionCache.y;
	int sx = (int)m_size.x;
	int sy = (int)m_size.y;

	if (x < nx || y < ny || (x > nx+sx) || (y > ny+sy))
		return false;

	if (m_pFlash && m_bVisible && m_bProcessInput)
		m_pFlash->injectMouseMove(x-nx, y-ny);

	return true;
}
//---------------------------------------------------------------------------
bool FlashElement::injectMouseDown(int button, int x, int y)
{
	// todo: check to see if the event is within bounds
	int nx = (int)m_positionCache.x;
	int ny = (int)m_positionCache.y;
	int sx = (int)m_size.x;
	int sy = (int)m_size.y;

	if (x < nx || y < ny || (x > nx+sx) || (y > ny+sy))
		return false;

	if (m_pFlash && m_bVisible && m_bProcessInput)
		m_pFlash->injectMouseDown(button, x-nx, y-ny);

	return true;
}
//---------------------------------------------------------------------------
bool FlashElement::injectMouseUp(int button, int x, int y)
{
	// todo: check to see if the event is within bounds
	int nx = (int)m_positionCache.x;
	int ny = (int)m_positionCache.y;
	int sx = (int)m_size.x;
	int sy = (int)m_size.y;

	if (x < nx || y < ny || (x > nx+sx) || (y > ny+sy))
		return false;

	if (m_pFlash && m_bVisible && m_bProcessInput)
		m_pFlash->injectMouseUp(button, x-nx, y-ny);

	return true;
}
//---------------------------------------------------------------------------
bool FlashElement::injectKeyDown(int keyCode, int keyChar)
{
	if (m_pFlash && m_bVisible && m_bProcessInput)
		m_pFlash->injectKeyDown(keyCode, keyChar);

	return true;
}
//---------------------------------------------------------------------------
bool FlashElement::injectKeyUp(int keyCode, int keyChar)
{
	if (m_pFlash && m_bVisible && m_bProcessInput)
		m_pFlash->injectKeyUp(keyCode, keyChar);

	return true;
}
//---------------------------------------------------------------------------
bool FlashElement::injectKeyEvent(size_t msg, size_t wParam, size_t lParam)
{
	if (m_pFlash && m_bVisible && m_bProcessInput)
		m_pFlash->injectKeyEvent(msg, wParam, lParam);

	return true;
}
//---------------------------------------------------------------------------
void FlashElement::call(const char* function, const _variant_t* argv, int argc)
{
	if (m_pFlash)
		m_pFlash->call(function, argv, argc);
}
