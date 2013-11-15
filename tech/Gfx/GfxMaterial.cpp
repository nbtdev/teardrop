/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/


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
TD_ENUM(GfxMaterial, CullMode, "Set culling mode for objects rendered with this material");
TD_ENUM_VALUE(GfxMaterial, CullMode, CULL_NONE, "No culling (double-sided rendering)");
TD_ENUM_VALUE(GfxMaterial, CullMode, CULL_CCW, "Counter-clockwise culling [default]");
TD_ENUM_VALUE(GfxMaterial, CullMode, CULL_CW, "Clockwise culling");
TD_ENUM(GfxMaterial, FillMode, "Set polygon fill mode for objects rendered with this material");
TD_ENUM_VALUE(GfxMaterial, FillMode, FILLMODE_NONE, "No fill");
TD_ENUM_VALUE(GfxMaterial, FillMode, FILLMODE_POINT, "Render vertices only");
TD_ENUM_VALUE(GfxMaterial, FillMode, FILLMODE_WIREFRAME, "Render edges only");
TD_ENUM_VALUE(GfxMaterial, FillMode, FILLMODE_SOLID, "Render solid polygons [default]");
//---------------------------------------------------------------------------
const size_t MAX_TEXTURE_STAGES = 8;
//---------------------------------------------------------------------------
GfxMaterial::GfxMaterial()
{
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

	return m_pTextureStages[index]->getEnabled();
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
//bool GfxMaterial::isLit() const
//{
//	return (m_numLights > 0);
//}
//---------------------------------------------------------------------------
size_t GfxMaterial::getNumLights() const
{
	return m_numLights;
}
//---------------------------------------------------------------------------
GfxMaterial::CustomShader GfxMaterial::getCustomShader() const
{
	return (CustomShader)m_special;
}
//---------------------------------------------------------------------------
bool GfxMaterial::isTransparent() const
{
	return (___DepthCheck && !___DepthWrite || ___AlphaBlending);
}
//---------------------------------------------------------------------------
void GfxMaterial::setNumLights(unsigned int numLights)
{
	m_numLights = unsigned char(numLights);
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
		if (pStage->getEnabled() && pStage->getTexture())
		{
			++numSamplers;
	
			// hash in the layer blend ops
		}
	}

	hash |= numSamplers;
	hash |= (blendOps << 2);

	// lighting
	if (getLit())
	{
		// hash in the number of lights desired
		hash |= (m_numLights << 14);
	}

	// vertex colors
	if (getVertexColors())
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
