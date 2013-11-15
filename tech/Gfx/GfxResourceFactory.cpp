/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/


#include "GfxResourceFactory.h"
#include "GfxMesh.h"
#include "GfxTexture.h"
#include "GfxRenderer.h"
#include "GfxCommon.h"
#include "Util/FourCC.h"
#include "Util/Environment.h"
#include <new.h>

using namespace Teardrop;
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
		return TD_NEW GfxMesh;
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
