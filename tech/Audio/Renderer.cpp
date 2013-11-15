/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Renderer.h"

using namespace Teardrop;
using namespace Audio;
//---------------------------------------------------------------------------
Renderer::Renderer()
{
}
//---------------------------------------------------------------------------
Renderer::~Renderer()
{
}
//---------------------------------------------------------------------------
Renderer* Renderer::createInstance()
{
	//RendererXAudio2* pRend = TD_NEW RendererXAudio2();
	//if (pRend->initialize())
	//	return pRend;

	return 0;
}
//---------------------------------------------------------------------------
void Renderer::destroyInstance(Renderer* pRenderer)
{
	if (pRenderer)
		pRenderer->destroy();

	delete pRenderer;
}
