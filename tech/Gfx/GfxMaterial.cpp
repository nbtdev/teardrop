/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "GfxMaterial.h"
#include "GfxTextureStage.h"
#include "GfxTexture.h"
#include "GfxCommon.h"
#include "Resource/ResourceManager.h"
#include "Serialization/ResourceSerializer.h"
#include "Util/Environment.h"
#include <assert.h>
#include <memory.h>
#include <new.h>

using namespace Teardrop;
//---------------------------------------------------------------------------
TD_CLASS_IMPL(GfxMaterial);
//---------------------------------------------------------------------------
const size_t MAX_TEXTURE_STAGES = 8;
//---------------------------------------------------------------------------
GfxMaterial::GfxMaterial()
{
//	memset(this, 0, sizeof(GfxMaterial));
	m_cullMode = CULL_CCW;
	m_fillMode = FILLMODE_SOLID;
	m_bDepthCheck = true;
	m_bDepthWrite = true;
}
//---------------------------------------------------------------------------
GfxMaterial::GfxMaterial(int i) 
{
	UNREFERENCED_PARAMETER(i);
	m_bHasAllTextures = false;
}
//---------------------------------------------------------------------------
GfxMaterial::~GfxMaterial()
{
}
//---------------------------------------------------------------------------
size_t GfxMaterial::getMaxNumTextureStages() const
{
	return MAX_TEXTURE_STAGES;
}
//---------------------------------------------------------------------------
size_t GfxMaterial::getNumTextureStages() const
{
	return m_pTextureStages.size();
}
//---------------------------------------------------------------------------
bool GfxMaterial::initialize()
{
	//for (size_t i=0; i<MAX_TEXTURE_STAGES; ++i)
	//{
	//	size_t blockSize = sizeof(GfxTextureStage);
	//	GfxTextureStage* pStage = 
	//		(GfxTextureStage*)GFX_ALLOCATE_ALIGNED(blockSize, 16);

	//	memset(pStage, 0, blockSize);
	//	m_pTextureStages.push_back(pStage);
	//}

	return true;
}
//---------------------------------------------------------------------------
bool GfxMaterial::destroy()
{
	for (size_t i=0; i<m_pTextureStages.size(); ++i)
	{
		m_pTextureStages[i]->destroy();
		GFX_FREE(m_pTextureStages[i]);
	}

	return true;
}
//---------------------------------------------------------------------------
unsigned __int64 GfxMaterial::getHashCode()
{
	checkForAllTextures();
	return m_hashCode;
}
//---------------------------------------------------------------------------
void GfxMaterial::checkForAllTextures()
{
	// this is sort of a hack to get around the fact that shaders are built
	// usually before all of the textures that the material needs, are loaded,
	// and this causes the sampling code not to be built into the shader. To
	// get around that, we add this code to the getHashCode() method, which
	// is called every time a material is used in rendering, and once all 
	// textures are in, we can redo the final hash and rebuild the shader.
	if (!m_bHasAllTextures)
	{
		bool hasAll = true;
		for (size_t i=0; i<getNumTextureStages(); ++i)
		{
			GfxTextureStage* pStage = m_pTextureStages[i];
			hasAll &= (pStage->getTexture() != 0);
		}

		m_bHasAllTextures = hasAll;

		if (hasAll)
		{
			// rehash us
			recalcHashCode();
		}
	}
}
//---------------------------------------------------------------------------
GfxTextureStage* GfxMaterial::addTextureStage()
{
	assert(m_pTextureStages.size() < MAX_TEXTURE_STAGES);
	if (m_pTextureStages.size() >= MAX_TEXTURE_STAGES)
	{
		return 0;
	}

	size_t blockSize = sizeof(GfxTextureStage);
	GfxTextureStage* pStage = 
		(GfxTextureStage*)GFX_ALLOCATE_ALIGNED(blockSize, 16);

	memset(pStage, 0, blockSize);
	m_pTextureStages.push_back(pStage);

	pStage->initialize();

	return m_pTextureStages[m_pTextureStages.size() - 1];
}
//---------------------------------------------------------------------------
const GfxTextureStage* GfxMaterial::getTextureStage(size_t index) const
{
	assert(index < m_pTextureStages.size());
	if (index >= m_pTextureStages.size())
	{
		return 0;
	}

	return m_pTextureStages[index];
}
//---------------------------------------------------------------------------
GfxTextureStage* GfxMaterial::getTextureStage(size_t index)
{
	assert(index < m_pTextureStages.size());
	if (index >= m_pTextureStages.size())
	{
		return 0;
	}

	return m_pTextureStages[index];
}
//---------------------------------------------------------------------------
bool GfxMaterial::isTextureStageEnabled(size_t index) const
{
	assert(index < m_pTextureStages.size());
	if (index >= m_pTextureStages.size())
	{
		return false;
	}

	return m_pTextureStages[index]->isEnabled();
}
//---------------------------------------------------------------------------
bool GfxMaterial::setTextureStage(size_t index, bool bEnabled)
{
	assert(index < m_pTextureStages.size());
	if (index >= m_pTextureStages.size())
	{
		return false;
	}

	m_pTextureStages[index]->setEnabled(bEnabled);
	recalcHashCode();
	return true;
}
////---------------------------------------------------------------------------
//unsigned int GfxMaterial::getDiffuse() const
//{
//	return m_diffuse;
//}
////---------------------------------------------------------------------------
//unsigned int GfxMaterial::getAmbient() const
//{
//	return m_ambient;
//}
////---------------------------------------------------------------------------
//unsigned int GfxMaterial::getSpecular() const
//{
//	return m_specular;
//}
////---------------------------------------------------------------------------
//unsigned int GfxMaterial::getEmissive() const
//{
//	return m_emissive;
//}
//---------------------------------------------------------------------------
bool GfxMaterial::isAlphaBlended() const
{
	return m_bAlphaBlend;
}
////---------------------------------------------------------------------------
//bool GfxMaterial::getDepthCheck() const
//{
//	return m_bDepthCheck;
//}
////---------------------------------------------------------------------------
//bool GfxMaterial::getDepthWrite() const
//{
//	return m_bDepthWrite;
//}
//---------------------------------------------------------------------------
unsigned char GfxMaterial::getDepthBias() const
{
	return m_depthBias;
}
////---------------------------------------------------------------------------
//bool GfxMaterial::getColorWrite() const
//{
//	return m_bColorWrite;
//}
//---------------------------------------------------------------------------
bool GfxMaterial::getVertexColors() const
{
	return m_bVertexColors;
}
//---------------------------------------------------------------------------
bool GfxMaterial::isLit() const
{
	return (m_numLights > 0);
}
//---------------------------------------------------------------------------
size_t GfxMaterial::getNumLights() const
{
	return m_numLights;
}
//---------------------------------------------------------------------------
GfxMaterial::CullMode GfxMaterial::getCullMode() const
{
	return m_cullMode;
}
//---------------------------------------------------------------------------
GfxMaterial::FillMode GfxMaterial::getFillMode() const
{
	return m_fillMode;
}
//---------------------------------------------------------------------------
GfxMaterial::CustomShader GfxMaterial::getCustomShader() const
{
	return (CustomShader)m_special;
}
//---------------------------------------------------------------------------
bool GfxMaterial::isTransparent() const
{
	return (m_bDepthCheck && !m_bDepthWrite || m_bAlphaBlend);
}
////---------------------------------------------------------------------------
//void GfxMaterial::setDiffuse(unsigned int c)
//{
//	m_diffuse = c;
//}
////---------------------------------------------------------------------------
//void GfxMaterial::setAmbient(unsigned int c)
//{
//	m_ambient = c;
//}
////---------------------------------------------------------------------------
//void GfxMaterial::setSpecular(unsigned int c)
//{
//	m_specular = c;
//}
////---------------------------------------------------------------------------
//void GfxMaterial::setEmissive(unsigned int c)
//{
//	m_emissive = c;
//}
//---------------------------------------------------------------------------
void GfxMaterial::setAlphaBlended(bool bBlend)
{
	m_bAlphaBlend = bBlend;
}
////---------------------------------------------------------------------------
//void GfxMaterial::setDepthCheck(bool bEnabled)
//{
//	m_bDepthCheck = bEnabled;
//}
////---------------------------------------------------------------------------
//void GfxMaterial::setDepthWrite(bool bEnabled)
//{
//	m_bDepthWrite = bEnabled;
//}
//---------------------------------------------------------------------------
void GfxMaterial::setDepthBias(unsigned char bias)
{
	m_depthBias = bias;
}
////---------------------------------------------------------------------------
//void GfxMaterial::setColorWrite(bool bEnabled)
//{
//	m_bColorWrite = bEnabled;
//}
//---------------------------------------------------------------------------
void GfxMaterial::setVertexColors(bool bEnabled)
{
	m_bVertexColors = bEnabled;
}
//---------------------------------------------------------------------------
void GfxMaterial::setNumLights(unsigned int numLights)
{
	m_numLights = unsigned char(numLights);
}
//---------------------------------------------------------------------------
void GfxMaterial::setCullMode(CullMode culling)
{
	m_cullMode = culling;
}
//---------------------------------------------------------------------------
void GfxMaterial::setFillMode(FillMode fillMode)
{
	m_fillMode = fillMode;
}
//---------------------------------------------------------------------------
void GfxMaterial::setCustomShader(CustomShader shader)
{
	m_special = unsigned short(shader);
}
//---------------------------------------------------------------------------
void GfxMaterial::notify()
{
	// since the layer blend ops are handled in the texture stage class,
	// we need a way for it to tell us those changed, if they did
}
//---------------------------------------------------------------------------
GfxMaterial& GfxMaterial::operator=(const GfxMaterial& other)
{
	// in general, we can just memcpy other to us; but we also 
	// need to duplicate the texture stage entries
	memcpy(this, &other, sizeof(*this));
	
	// we just want to clear out *our* version, we don't want to try to 
	// cleanup *their* version in the process
	m_pTextureStages._init(); 

	for (size_t i=0; i<other.getNumTextureStages(); ++i)
	{
		GfxTextureStage* pStage = addTextureStage();
		const GfxTextureStage* pOtherStage = other.getTextureStage(i);

		memcpy(pStage, pOtherStage, sizeof(GfxTextureStage));

		// inc ref count
		Environment::get().pResourceMgr->deref(pStage->getTextureHandle());
	}

	return *this;
}
//---------------------------------------------------------------------------
void GfxMaterial::recalcHashCode()
{
	/*
		material hash code is similar in purpose to the submesh hash code -
		it allows for fast lookup of the shaders needed to implement the 
		shading defined in this material. The main criteria that define which
		shader to use for a given material are:

		* texture samplers used (max 4), if any              - 2 bits
		* layer blend ops in use if more than one texture    - 3 bits ea (12)
		* lighting state (lit or unlit, # lights, max 4)     - 2 bits
		* lighting model (if lit), i.e. Lambert, Phong, etc. - 3 bits
		* vertex colors used (tied to submesh data)          - 1 bit
		* which material colors to use (if any)              - 4 bits
		* normal mapping (if used)                           - 1 bit
		* environment mapping (if used)                      - 1 bit
		* diffuse mapping (if used)                          - 1 bit
		* specular mapping (if used)                         - 1 bit
		* other specialty effects                            - 4 bits
                                                             ---------
															  32 bits
	*/

	unsigned int hash = 0;

	// texture samplers
	unsigned int numSamplers = 0;
	unsigned int blendOps = 0;
	for (size_t i=0; i<m_pTextureStages.size(); ++i)
	{
		GfxTextureStage* pStage = m_pTextureStages[i];
		if (pStage->isEnabled() && pStage->getTexture())
		{
			++numSamplers;
	
			// hash in the layer blend ops
		}
	}

	hash |= numSamplers;
	hash |= (blendOps << 2);

	// lighting
	if (isLit())
	{
		// hash in the number of lights desired
		hash |= (m_numLights << 14);
	}

	// vertex colors
	if (m_bVertexColors)
	{
		hash |= ((unsigned int)1 << 19);
	}

	m_hashCode = hash;

	// mark off any specialty/custom shaders in the upper half of the hash
	m_hashCode |= ((__int64)1 << (32 + m_special));
}
//---------------------------------------------------------------------------
bool GfxMaterial::serialize(ResourceSerializer& serializer)
{
	serializer.relocate(this, sizeof(GfxMaterial));

	// deal with the texture stage array...
	serializer.relocate(m_pTextureStages);

	// and then write out each texture stage def
	for (size_t i=0; i<m_pTextureStages.size(); ++i)
	{
		m_pTextureStages[i]->serialize(serializer);
	}

	return true;
}
