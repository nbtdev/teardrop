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
#include "UIRenderer.h"
#include "UIElement.h"
#include "Gfx/include/GfxRenderer.h"
#include "Gfx/include/GfxViewport.h"
#include <assert.h>

using namespace CoS;
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
