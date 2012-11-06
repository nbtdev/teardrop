/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "UIQuadElement.h"
#include "UIRenderer.h"
#include "Gfx/GfxMesh.h"
#include "Gfx/GfxSubMesh.h"
#include "Gfx/GfxVertexData.h"
#include "Gfx/GfxVertexFormat.h"
#include "Gfx/GfxMaterial.h"
#include "Gfx/GfxUtil.h"
#include "Gfx/GfxTextureStage.h"
#include "Resource/ResourceManager.h"
#include "Util/Environment.h"
#include <assert.h>

using namespace Teardrop;
using namespace UI;
//---------------------------------------------------------------------------
QuadElement::QuadElement()
{
	m_hQuad = INVALID_RESOURCE_HANDLE;
	m_pMtl = 0;
}
//---------------------------------------------------------------------------
QuadElement::~QuadElement()
{
}
//---------------------------------------------------------------------------
static float s_quadVerts[] = 
{
	/*3D position*/	/*texcoord*/
	0, 0, 1,		0, 0,
	0, 1, 1,		0, 1,
	1, 0, 1,		1, 0,
	1, 1, 1,		1, 1,
};
//---------------------------------------------------------------------------
bool QuadElement::initialize(HResource hTexture, const ElementParams& params)
{
	// get a GfxMesh from the resource manager
	m_hQuad = Environment::get().pResourceMgr->acquire(GfxMesh::RESOURCE_TYPE);

	// assumption: this is immediately available when we create one with no name
	GfxMesh* pMesh = AutoResPtr<GfxMesh>(m_hQuad);
	assert(pMesh);

	if (!pMesh)
		return false;

	GfxSubMesh* pSM = pMesh->createSubMesh();

	GfxVertexFormat fmt;
	GfxVertexFormat::Element elem;
	elem.semantic = POSITION;
	elem.type = FLOAT3;
	fmt.addElement(elem);
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
		GfxVertexData::DYNAMIC, // because we might have to change it later
		s_quadVerts);
	pSM->setPrimitiveType(TRISTRIP);

	// create the material that goes along with this thing
	m_pMtl = TD_NEW GfxMaterial;
	m_pMtl->initialize();
	GfxTextureStage* pStage = m_pMtl->addTextureStage();
	pStage->setEnabled(true);
	pStage->setTexture(0, hTexture);

	pSM->setMaterial(m_pMtl);

	return true;
}
//---------------------------------------------------------------------------
void QuadElement::destroy()
{
	Environment::get().pResourceMgr->release(m_hQuad);
	
	if (m_pMtl)
		m_pMtl->destroy();

	delete m_pMtl;
}
//---------------------------------------------------------------------------
HResource QuadElement::getMeshHandle()
{
	return m_hQuad;
}
//---------------------------------------------------------------------------
bool QuadElement::render(Renderer* pRenderer)
{
	return true;
}
//---------------------------------------------------------------------------
bool QuadElement::injectMouseMove(int x, int y)
{
	return false;
}
//---------------------------------------------------------------------------
bool QuadElement::injectMouseDown(int button, int x, int y)
{
	return false;
}
//---------------------------------------------------------------------------
bool QuadElement::injectMouseUp(int button, int x, int y)
{
	return false;
}
//---------------------------------------------------------------------------
bool QuadElement::injectKeyDown(int keyCode, int keyChar)
{
	return false;
}
//---------------------------------------------------------------------------
bool QuadElement::injectKeyUp(int keyCode, int keyChar)
{
	return false;
}
//---------------------------------------------------------------------------
bool QuadElement::injectKeyEvent(size_t msg, size_t wParam, size_t lParam)
{
	return false;
}
//---------------------------------------------------------------------------
void QuadElement::call(const char* function, const _variant_t* argv, int argc)
{
}
