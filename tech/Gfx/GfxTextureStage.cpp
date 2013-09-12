/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "GfxTextureStage.h"
#include "GfxTexture.h"
#include "GfxCommon.h"
#include "Serialization/ResourceSerializer.h"
#include "Resource/ResourceManager.h"
#include "Util/Environment.h"
#include <assert.h>
#include <new.h>

using namespace Teardrop;
TD_CLASS_IMPL(GfxTextureStage);
TD_ENUM(GfxTextureStage, Filter, "Min/Mag/Mip filter values");
TD_ENUM_VALUE(GfxTextureStage, Filter, FILTER_NONE, "No filtering");
TD_ENUM_VALUE(GfxTextureStage, Filter, FILTER_NEAREST, "Nearest-point filtering");
TD_ENUM_VALUE(GfxTextureStage, Filter, FILTER_BILINEAR, "Bilinear filtering");
TD_ENUM_VALUE(GfxTextureStage, Filter, FILTER_ANISOTROPIC, "Anisotropic filtering");
TD_ENUM(GfxTextureStage, AddressMode, "Texture addressing modes");
TD_ENUM_VALUE(GfxTextureStage, AddressMode, ADDRMODE_UNSET, "Not set");
TD_ENUM_VALUE(GfxTextureStage, AddressMode, ADDRMODE_WRAP, "Wrap addressing");
TD_ENUM_VALUE(GfxTextureStage, AddressMode, ADDRMODE_MIRROR, "Mirror addressing");
TD_ENUM_VALUE(GfxTextureStage, AddressMode, ADDRMODE_CLAMP, "Clamp addressing");
TD_ENUM_VALUE(GfxTextureStage, AddressMode, ADDRMODE_BORDER, "Border addressing");
TD_ENUM(GfxTextureStage, BlendMode, "Texture layer blending mode");
TD_ENUM_VALUE(GfxTextureStage, BlendMode, BLENDMODE_REPLACE, "Replace pixels from previous layer");
TD_ENUM_VALUE(GfxTextureStage, BlendMode, BLENDMODE_MODULATE, "Multiply with pixels in previous layer");
TD_ENUM_VALUE(GfxTextureStage, BlendMode, BLENDMODE_ADD, "Add to pixels in previous layer");
TD_ENUM(GfxTextureStage, MapHint, "Hints for how to apply texture map");
TD_ENUM_VALUE(GfxTextureStage, MapHint, MAP_UNKNOWN, "Not set");
TD_ENUM_VALUE(GfxTextureStage, MapHint, MAP_DIFFUSE, "Use map for diffuse colors");
TD_ENUM_VALUE(GfxTextureStage, MapHint, MAP_SPECULAR, "Use texture for specular mapping");
TD_ENUM_VALUE(GfxTextureStage, MapHint, MAP_NORMAL, "Use texture for normal mapping");
TD_ENUM_VALUE(GfxTextureStage, MapHint, MAP_AMBIENT, "Use texture for ambient/occlusion mapping");
TD_ENUM_VALUE(GfxTextureStage, MapHint, MAP_ILLUMINATION, "Use texture for light mapping");
TD_ENUM_VALUE(GfxTextureStage, MapHint, MAP_IRRADIANCE, "Use texture for radiance mapping");
TD_ENUM_VALUE(GfxTextureStage, MapHint, MAP_ENVIRONMENT, "Use texture for environment mapping");
TD_ENUM_VALUE(GfxTextureStage, MapHint, MAP_DISPLACEMENT, "Use texture for displacement mapping");
//---------------------------------------------------------------------------
GfxTextureStage::GfxTextureStage()
{
	m_hTexture = INVALID_RESOURCE_HANDLE;
}
//---------------------------------------------------------------------------
GfxTextureStage::~GfxTextureStage()
{
}
//---------------------------------------------------------------------------
bool GfxTextureStage::initialize()
{
	m_hTexture = INVALID_RESOURCE_HANDLE;
	return true;
}
//---------------------------------------------------------------------------
bool GfxTextureStage::destroy()
{
	return true;
}
//---------------------------------------------------------------------------
GfxTexture* GfxTextureStage::getTexture()
{
	if (!m_pTexture)
	{
		return AutoResPtr<GfxTexture>(m_hTexture);
	}

	return m_pTexture;
}
//---------------------------------------------------------------------------
HResource GfxTextureStage::getTextureHandle() const
{
	return m_hTexture._u.pC;
}
//---------------------------------------------------------------------------
void GfxTextureStage::setTexture(GfxTexture* pTexture, HResource texHandle)
{
	m_pTexture = pTexture;
	m_hTexture = texHandle;
}
//---------------------------------------------------------------------------
// we do this because the warning is immaterial -- on all platforms on which 
// this particular code will run (Win32), the two are the same size
#pragma warning(push)
#pragma warning(disable: 4311 4312)
void GfxTextureStage::setTextureName(const char* name)
{
	m_pName = reinterpret_cast<unsigned int>(name);
}
//---------------------------------------------------------------------------
const char* GfxTextureStage::getTextureName() const
{
	return reinterpret_cast<const char*>(m_pName);
}
#pragma warning(pop)
//---------------------------------------------------------------------------
bool GfxTextureStage::serialize(ResourceSerializer& serializer)
{
	// write out the stage bits
	serializer.relocate(this, sizeof(GfxTextureStage));

	// and then the texture pointer reloc

	if (m_pTexture)
	{
		// not likely ever to happen in any context in which this method runs
		serializer.relocate(m_pTexture);
		m_pTexture->serialize(serializer);
	}
	else
	{
		// likely to happen -- the tool generates a external ref that is
		// linked up later to a live texture
		serializer.relocate(m_hTexture, getTextureName());
	}

	return true;
}
