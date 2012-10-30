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
#include "GfxResourceFactory.h"
#include "GfxMesh.h"
#include "GfxTexture.h"
#include "GfxRenderer.h"
#include "GfxCommon.h"
#include "Util/include/FourCC.h"
#include "Util/include/Environment.h"
#include <new.h>

using namespace CoS;
//---------------------------------------------------------------------------
GfxResourceFactory::GfxResourceFactory(Environment& env)
: m_env(env)
{
}
//---------------------------------------------------------------------------
GfxResourceFactory::~GfxResourceFactory()
{
}
//---------------------------------------------------------------------------
Resource* GfxResourceFactory::createPlaceholder(const FourCC& /*fourCC*/)
{
	return 0;
}
//---------------------------------------------------------------------------
Resource* GfxResourceFactory::create(const FourCC& fourCC)
{
	if (fourCC == GfxMesh::RESOURCE_TYPE)
	{
		return COS_NEW GfxMesh;
	}

	if (fourCC == GfxTexture::RESOURCE_TYPE)
	{
		return m_env.pRenderer->createTexture();
	}

	return 0;
}
//---------------------------------------------------------------------------
void GfxResourceFactory::destroy(Resource* pRes, const FourCC& fourCC)
{
	if (fourCC == GfxMesh::RESOURCE_TYPE)
	{
		GfxMesh* pMesh = static_cast<GfxMesh*>(pRes);
		pMesh->destroy();
		delete pMesh;
	}

	if (fourCC == GfxTexture::RESOURCE_TYPE)
	{
		GfxTexture* pTex = static_cast<GfxTexture*>(pRes);
		pTex->destroy();
		delete pTex;
	}
}
//---------------------------------------------------------------------------
Allocator* GfxResourceFactory::getAllocator()
{
	return getGfxAllocator();
}
