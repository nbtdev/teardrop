/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "UIQuadRenderer.h"
#include "UIQuadElement.h"
#include "Gfx/GfxRenderer.h"
#include "Gfx/GfxViewport.h"
#include <assert.h>

using namespace Teardrop;
using namespace UI;
//---------------------------------------------------------------------------
QuadRenderer::QuadRenderer()
{
}
//---------------------------------------------------------------------------
QuadRenderer::~QuadRenderer()
{
}
//---------------------------------------------------------------------------
bool QuadRenderer::initialize(GfxRenderer* pRenderer)
{
	return Renderer::initialize(pRenderer);
}
//---------------------------------------------------------------------------
bool QuadRenderer::destroy()
{
	return Renderer::destroy();
}
//---------------------------------------------------------------------------
bool QuadRenderer::render(Element* pElement)
{
	return true;
}
