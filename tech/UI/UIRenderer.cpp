/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "UIRenderer.h"
#include "UIElement.h"
#include "Gfx/GfxRenderer.h"
#include "Gfx/GfxViewport.h"
#include <assert.h>

using namespace Teardrop;
using namespace UI;
//---------------------------------------------------------------------------
Renderer::Renderer()
{
	m_pRenderer = 0;
	m_pViewport = 0;
}
//---------------------------------------------------------------------------
Renderer::~Renderer()
{
}
//---------------------------------------------------------------------------
bool Renderer::initialize(GfxRenderer* pRenderer)
{
	m_pRenderer = pRenderer;
	return true;
}
//---------------------------------------------------------------------------
bool Renderer::destroy()
{
	return true;
}
//---------------------------------------------------------------------------
void Renderer::setViewport(GfxViewport* pVP)
{
	//assert(m_pRenderer);
	//if (!m_pRenderer)
	//	return;

	//m_pViewport = pVP;

	//// set the render target to this viewport's RT
	//m_pRenderer->setRenderTarget(pVP->getRenderTarget());
}
//---------------------------------------------------------------------------
bool Renderer::render(Element* pElement)
{
	return true;
}
