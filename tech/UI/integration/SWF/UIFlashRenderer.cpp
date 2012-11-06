/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "UIFlashRenderer.h"
#include "UIFlashElement.h"
#include "Gfx/GfxRenderer.h"
#include "Gfx/GfxViewport.h"
#include <assert.h>

using namespace Teardrop;
using namespace UI;
//---------------------------------------------------------------------------
FlashRenderer::FlashRenderer()
{
}
//---------------------------------------------------------------------------
FlashRenderer::~FlashRenderer()
{
}
//---------------------------------------------------------------------------
bool FlashRenderer::initialize(GfxRenderer* pRenderer)
{
	return Renderer::initialize(pRenderer);
}
//---------------------------------------------------------------------------
bool FlashRenderer::destroy()
{
	return Renderer::destroy();
}
//---------------------------------------------------------------------------
bool FlashRenderer::render(Element* pElement)
{
	m_pRenderer->queueForRendering(pElement->getMeshInstance());
	return true;
}
