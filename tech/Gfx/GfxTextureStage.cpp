/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/


#include "GfxTextureStage.h"
#include "GfxTexture.h"
#include "GfxCommon.h"
#include "Util/Environment.h"
#include <assert.h>

using namespace Teardrop;
TD_CLASS_IMPL(GfxTextureStage);
//---------------------------------------------------------------------------
GfxTextureStage::GfxTextureStage()
    : mTexture(nullptr)
{
}
//---------------------------------------------------------------------------
GfxTextureStage::~GfxTextureStage()
{
}
//---------------------------------------------------------------------------
bool GfxTextureStage::initialize()
{
	return true;
}
//---------------------------------------------------------------------------
bool GfxTextureStage::destroy()
{
	return true;
}
//---------------------------------------------------------------------------
GfxTexture* GfxTextureStage::texture()
{
    return mTexture;
}
//---------------------------------------------------------------------------
void GfxTextureStage::setTexture(GfxTexture* pTexture)
{
    mTexture = pTexture;
}
