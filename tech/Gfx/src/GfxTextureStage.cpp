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
#include "GfxTextureStage.h"
#include "GfxTexture.h"
#include "GfxCommon.h"
#include "Serialization/include/ResourceSerializer.h"
#include "Resource/include/ResourceManager.h"
#include "Util/include/Environment.h"
#include <assert.h>
#include <new.h>

using namespace CoS;
//---------------------------------------------------------------------------
DEFINE_SERIALIZABLE(GfxTextureStage);
//---------------------------------------------------------------------------
GfxTextureStage::GfxTextureStage()
{
	memset(this, 0, sizeof(GfxTextureStage));
	m_hTexture = INVALID_RESOURCE_HANDLE;
}
//---------------------------------------------------------------------------
GfxTextureStage::GfxTextureStage(int i)
{
	UNREFERENCED_PARAMETER(i);
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
	Environment::get().pResourceMgr->release(m_hTexture);
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
void GfxTextureStage::getFilter(
	Filter& minFilter, Filter& magFilter, Filter& mipFilter) const
{
	minFilter = m_minFilter;
	magFilter = m_magFilter;
	mipFilter = m_mipFilter;
}
//---------------------------------------------------------------------------
void GfxTextureStage::getTextureAddressing(
	AddressMode& U, AddressMode& V, AddressMode& W) const
{
	U = m_addrU;
	V = m_addrV;
	W = m_addrW;
}
//---------------------------------------------------------------------------
unsigned int GfxTextureStage::getTexCoordSet() const
{
	return m_texCoordSet;
}
//---------------------------------------------------------------------------
GfxTextureStage::BlendMode GfxTextureStage::getLayerBlendMode() const
{
	return m_blendMode;
}
//---------------------------------------------------------------------------
GfxTextureStage::MapHint GfxTextureStage::getMapHint() const
{
	return m_mapHint;
}
//---------------------------------------------------------------------------
bool GfxTextureStage::isEnabled() const
{
	return (m_enabled == 1);
}
//---------------------------------------------------------------------------
void GfxTextureStage::setFilter(
	Filter minFilter, Filter magFilter, Filter mipFilter)
{
	m_minFilter = minFilter;
	m_magFilter = magFilter;
	m_mipFilter = mipFilter;
}
//---------------------------------------------------------------------------
void GfxTextureStage::setTextureAddressing(
	AddressMode U, AddressMode V, AddressMode W)
{
	m_addrU = U;
	m_addrV = V;
	m_addrW = W;
}
//---------------------------------------------------------------------------
void GfxTextureStage::setTexCoordSet(unsigned int set)
{
	m_texCoordSet = (set == 0xFFFFFFFF) ? 0 : set;
}
//---------------------------------------------------------------------------
void GfxTextureStage::setLayerBlendMode(BlendMode mode)
{
	m_blendMode = mode;
}
//---------------------------------------------------------------------------
void GfxTextureStage::setMapHint(MapHint hint)
{
	m_mapHint = hint;
}
//---------------------------------------------------------------------------
void GfxTextureStage::setEnabled(bool bEnabled)
{
	m_enabled = bEnabled ? 1 : 0;
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
