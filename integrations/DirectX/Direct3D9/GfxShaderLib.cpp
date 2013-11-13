/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "GfxShaderLib.h"
#include "GfxRenderer.h"
#include "GfxRendererD3D9.h"
#include "GfxSubMesh.h"
#include "GfxMaterial.h"
#include "GfxTextureStage.h"
#include "GfxTexture.h"
#include "shader/GfxShaderCode.h"
#include "shader/GfxShaderD3D9.h"
#include "Resource/ResourceManager.h"
#include "Stream/FileStream.h"
#include "Util/Logger.h"
#include "Util/Hash.h"
#include "Util/Environment.h"

#include <assert.h>
#include <sstream>

using namespace Teardrop;
//---------------------------------------------------------------------------
static bool createVertexShaderSource(
	GfxSubMesh* pSubMesh, GfxMaterial* pMaterial, std::stringstream& source);
static bool createPixelShaderSource(
	GfxSubMesh* pSubMesh, GfxMaterial* pMaterial, std::stringstream& source);
static bool loadCustomShaderSource(/*in*/const char* shaderName,
	/*out*/String& source, /*out*/String& fullpath);
static bool getShaderSource(
	/*in*/GfxSubMesh* pSubMesh, 
	/*in*/GfxMaterial* pMaterial, 
	/*in*/const char* pFilename,
	/*out*/String& source,
	/*out*/String& techniqueName);
//---------------------------------------------------------------------------
const char* VS_ENTRY = "VS_MAIN";
const char* VS_PROFILE = "vs_3_0";
const char* PS_ENTRY = "PS_MAIN";
const char* PS_PROFILE = "ps_3_0";
//---------------------------------------------------------------------------
GfxShaderLib::GfxShaderLib(Environment& e) : m_env(e)
{
	m_pEffectPool = 0;
}
//---------------------------------------------------------------------------
GfxShaderLib::~GfxShaderLib()
{
}
//---------------------------------------------------------------------------
bool GfxShaderLib::initialize()
{
	return true;
}
//---------------------------------------------------------------------------
bool GfxShaderLib::destroy()
{
	// release all of our remaining shaders
	for (ShaderLUT::iterator it = m_lut.begin(); it != m_lut.end(); ++it)
	{
		it->second->destroy();
		delete it->second;
	}

	return true;
}
//---------------------------------------------------------------------------
bool getShaderSource(
	GfxSubMesh* pSubMesh,
	GfxMaterial* pMaterial, 
	const char* pFilename,
	String& source,
	String& fullpath)
{
	//if (pMaterial->getCustomShader() != GfxMaterial::SHADER_NO_CUSTOM)
	if (pFilename)
	{
		if (loadCustomShaderSource(
			pFilename,
			source, 
			fullpath))
		{
			return true;
		}
	}

	// otherwise, try a procedural shader
	fullpath.clear();
	std::stringstream ss;
	if (createVertexShaderSource(pSubMesh, pMaterial, ss))
	{
		if (createPixelShaderSource(pSubMesh, pMaterial, ss))
		{
			// need to add the tech/pass/shader parts to it, since this was
			// generated
			ss << std::endl << "technique T0 {";
			ss << std::endl << "\tpass P0 {";
			ss << std::endl << "\t\tVertexShader = compile vs_2_0 VS_MAIN();";
			ss << std::endl << "\t\tPixelShader = compile ps_2_0 PS_MAIN();";
			ss << std::endl << "\t}";
			ss << std::endl << "}" << std::endl;

			source = ss.str().c_str();
			Environment::get().pLogger->logMessage(source);
			return true;
		}
	}

	return false;
}
//---------------------------------------------------------------------------
GfxShaderD3D9* GfxShaderLib::findShader(unsigned __int64 hashCode) const
{
	ShaderLUT::const_iterator it = m_lut.find(hashCode);
	if (it != m_lut.end())
	{
		return it->second;
	}

	return 0;
}
//---------------------------------------------------------------------------
GfxShaderD3D9* GfxShaderLib::findShader(
	GfxSubMesh* pSubMesh, 
	GfxMaterial* pMaterial, 
	const char* pFilename,
	unsigned __int64& hash) const
{
	if (!pSubMesh || !pMaterial)
	{
		return 0;
	}

	unsigned __int64 hashes[3];
	hashes[0] = pSubMesh->getHashCode();
	hashes[1] = pMaterial->getHashCode();

	// filename comes with the VS and PS entry points, so it's entirely unique
	hashes[2] = hashString64(pFilename);
	hash = hashData64(hashes, sizeof(hashes));

	return findShader(hash);
}
//---------------------------------------------------------------------------
GfxShaderD3D9* GfxShaderLib::createShader(
	GfxSubMesh* pSubMesh, GfxMaterial* pMaterial, const char* pFilename)
{
	assert(pSubMesh);
	assert(pMaterial);

	// check to see if we already have one for this combo
	unsigned __int64 hash;
	GfxShaderD3D9* pShader = findShader(pSubMesh, pMaterial, pFilename, hash);
	if (!pShader)
	{
		// else we need to make one fresh 
		GfxShaderD3D9* p = TD_NEW GfxShaderD3D9(m_pEffectPool);

		String source;
		String errs;
		String fullpath;
		if (getShaderSource(pSubMesh, pMaterial, pFilename, source, fullpath))
		{
			m_lut[hash] = p;
			pShader = p;

			if (!p->initialize(source, errs))
			{
				Environment::get().pLogger->logMessage("Could not initialize shader, objects using this shader will not be rendered");
			}

			p->setFullPath(fullpath);
		}
	}
	
	return pShader;
}
//---------------------------------------------------------------------------
static void addNormalTransformCall(
	GfxSubMesh* pSubMesh, std::stringstream& source)
{
	if (pSubMesh->getVertexFormat().getElementBySemantic(NORMAL).semantic != UNUSED)
	{
		source << "\tpsIN.norm = doNormal((float3x3)WorldViewInv, vsIN.norm);" << std::endl;
	}
}
//---------------------------------------------------------------------------
static size_t addTexCoordTransformCall(
	GfxSubMesh* /*pSubMesh*/, 
	GfxMaterial* /*pMtl*/, 
	std::stringstream& /*source*/)
{
	// todo when we add texture effects such as scrolls and anims
	return 0;
}
//---------------------------------------------------------------------------
static size_t addTexCoordPassthru(
	GfxSubMesh* pSubMesh, 
	GfxMaterial* /*pMtl*/, 
	std::stringstream& source)
{
	// just pass each texcoord thru to the PS -- we know that we put the 
	// texcoords first in the PS_IN struct, so they are numbered in sequence
	// starting at zero; we return the next texcoord output in line.
	size_t idx = 0;

	const GfxVertexFormat& fmt = pSubMesh->getVertexFormat();

	for (size_t i=0; i < GfxVertexFormat::MAX_ELEMENT_COUNT; ++i)
	{
		const GfxVertexFormat::Element& elem = fmt.getElementAt(i);
		if (elem.semantic == TEXCOORD)
		{
			source 
				<< "\tvsIN.tex" << (size_t)elem.getUsage() << ".y = 1 - vsIN.tex" << (size_t)elem.getUsage() << ".y;" << std::endl
				<< "\tpsIN.tex" << (size_t)elem.getUsage()
				<< " = vsIN.tex" << (size_t)elem.getUsage() 
				<< ";" 
				<< std::endl;

			++idx;
		}
	}

	return idx;
}
//---------------------------------------------------------------------------
static void addVertexElements(GfxSubMesh* pSubMesh, std::stringstream& source)
{
	const GfxVertexFormat& fmt = pSubMesh->getVertexFormat();
	size_t numElems = fmt.getNumElements();
	for (size_t i=0; i<numElems; ++i)
	{
		const GfxVertexFormat::Element& elem = fmt.getElementAt(i);
		switch(elem.semantic)
		{
		case TEXCOORD:
			{
				size_t usage = (size_t)elem.getUsage();
				source << "\tfloat4 tex";
				source << usage;
				source << " : TEXCOORD";
				source << usage;
				source << ";";
				source << std::endl;
			}
			break;

		case COLOR:
			{
				size_t usage = (size_t)elem.getUsage();
				source << "\tfloat4 col";
				source << usage;
				source << " : COLOR";
				source << usage;
				source << ";";
				source << std::endl;
			}
			break;

		case NORMAL:
			source << "\tfloat4 norm : NORMAL;" << std::endl;
			break;

		case BLENDINDEX:
			source << "\tfloat4 blendIdx : BLENDINDICES;" << std::endl;
			break;

		case BLENDWEIGHT:
			source << "\tfloat4 blendWeight : BLENDWEIGHT;" << std::endl;
			break;
		}
	}
}
//---------------------------------------------------------------------------
static void addVSOutputElements(
	GfxSubMesh* pSubMesh, GfxMaterial* pMtl, std::stringstream& source)
{
	const GfxVertexFormat& fmt = pSubMesh->getVertexFormat();
	size_t idx = 0;

	// do texcoords first
	for (int i=0; i<8; ++i)
	{
		const GfxVertexFormat::Element& elem = fmt.getElementBySemantic(TEXCOORD, i);
		if (elem.semantic == UNUSED)
		{
			break;
		}

		source << "\tfloat4 tex";
		source << i;
		source << " : TEXCOORD";
		source << idx;
		source << ";";
		source << std::endl;

		++idx;
	}

	// then colors
	for (int i=0; i<2; ++i)
	{
		const GfxVertexFormat::Element& elem = fmt.getElementBySemantic(COLOR, i);
		if (elem.semantic == UNUSED)
		{
			break;
		}

		source << "\tfloat4 col";
		source << i;
		source << " : COLOR";
		source << idx;
		source << ";";
		source << std::endl;

		++idx;
	}

	// then normal and light vector(s), if lit
	if (pMtl->getLit())
	{
		const GfxVertexFormat::Element& elem = fmt.getElementBySemantic(NORMAL);
		if (elem.semantic != UNUSED)
		{
			source << "\tfloat4 norm : TEXCOORD";
			source << idx;
			source << ";";
			source << std::endl;

			++idx;

			// light vector interpolators
			for (int i=0; i<4; ++i)
			{
				source << "\tfloat4 light" << i << "vec : TEXCOORD";
				source << idx;
				source << ";";
				source << std::endl;

				++idx;
			}

			source << "\tfloat3 worldView : TEXCOORD";
			source << idx;
			source << ";";
			source << std::endl;

			++idx;
		}
	}

}
//---------------------------------------------------------------------------
static bool createVertexShaderSource(
	GfxSubMesh* pSubMesh, 
	GfxMaterial* pMaterial, 
	std::stringstream& source)
{
	//source.str("");
	source << GfxShaderCode::vs[GfxShaderCode::VS_3_0][GfxShaderCode::VS_GLOBALS];

	source << GfxShaderCode::vs[GfxShaderCode::VS_3_0][GfxShaderCode::VS_IN_OPEN];
	addVertexElements(pSubMesh, source);
	source << GfxShaderCode::vs[GfxShaderCode::VS_3_0][GfxShaderCode::VS_IN_CLOSE];
	source << GfxShaderCode::ps[GfxShaderCode::PS_3_0][GfxShaderCode::PS_IN_OPEN];
	addVSOutputElements(pSubMesh, pMaterial, source);
	source << GfxShaderCode::ps[GfxShaderCode::PS_3_0][GfxShaderCode::PS_IN_CLOSE];
	source << GfxShaderCode::vs[GfxShaderCode::VS_3_0][GfxShaderCode::VS_WVP_TRANSFORM];
	source << GfxShaderCode::vs[GfxShaderCode::VS_3_0][GfxShaderCode::VS_POS_SKINNED];
	source << GfxShaderCode::vs[GfxShaderCode::VS_3_0][GfxShaderCode::VS_NORM_SKINNED];
	source << GfxShaderCode::vs[GfxShaderCode::VS_3_0][GfxShaderCode::VS_CALC_LIGHT_VEC];

	if (pMaterial->getLit() && 
		pSubMesh->getVertexFormat().getElementBySemantic(NORMAL).semantic != UNUSED)
	{
		source << GfxShaderCode::vs[GfxShaderCode::VS_3_0][GfxShaderCode::VS_NORMAL_TRANSFORM];
	}

#if 0
	if (0) // for when we implement texture effects (animation, scrolling, etc)
	{
		source << GfxShaderCode::vs[GfxShaderCode::VS_3_0][GfxShaderCode::VS_TEXCOORD_TRANSFORM];
	}
#endif
	source << GfxShaderCode::vs[GfxShaderCode::VS_3_0][GfxShaderCode::VS_MAIN_OPEN];

	if (pSubMesh->getVertexFormat().hasColor())
	{
		source << GfxShaderCode::vs[GfxShaderCode::VS_3_0][GfxShaderCode::VS_COPY_COLOR];
	}

	// transform the position	
	if (pSubMesh->getVertexFormat().hasBlendindex())
	{
		source << GfxShaderCode::vs[GfxShaderCode::VS_3_0][GfxShaderCode::VS_SKIN_POS];
	}
	else
	{
		source << GfxShaderCode::vs[GfxShaderCode::VS_3_0][GfxShaderCode::VS_WVP_POS];
	}

	//
	// add bits and bobs if needed
	//

	// this will call either the texture transform (if present) or just add the passthru
	size_t idx = addTexCoordTransformCall(pSubMesh, pMaterial, source);
	if (!idx)
	{
		idx = addTexCoordPassthru(pSubMesh, pMaterial, source);
	}

	// pass through colors next...

	// and then normals, if any...per-pixel lighting you know...
	if (pMaterial->getLit())
	{
		if (pSubMesh->getVertexFormat().hasBlendindex())
		{
			source << GfxShaderCode::vs[GfxShaderCode::VS_3_0][GfxShaderCode::VS_SKIN_NORM];
		}
		else
		{
			addNormalTransformCall(pSubMesh, source);
		}

		// get lighting vectors, etc -- for now, assume all lights will be 
		// used; this can be optimized later if it becomes a problems
		for (int i=0; i<4; ++i)
		{
			// light vectors
			source << "\tpsIN.light" << i << "vec = mul(getLightVec(psIN.hPos, LightPos[";
			source << i << "], LightDir[" << i << "]), WorldViewInv);" << std::endl;
		}

		// worldview vector
		//source << "\tpsIN.worldView = normalize(float3(ViewInv[0].w, ViewInv[1].w, ViewInv[2].w) - psIN.hPos.xyz);\n";
		source << "\tpsIN.worldView = normalize(float3(ViewInv[0].w, ViewInv[1].w, ViewInv[2].w));\n";
	}

	source << GfxShaderCode::vs[GfxShaderCode::VS_3_0][GfxShaderCode::VS_MAIN_CLOSE]; 

	source << std::endl;

	return true;
}
//---------------------------------------------------------------------------
static void addSamplers(
	GfxMaterial* pMtl, std::stringstream& source)
{
	bool samp2D = false;
	bool samp3D = false;
	bool sampCUBE = false;

	for (size_t i=0; i<pMtl->getNumTextureStages(); ++i)
	{
		GfxTextureStage* pStage = pMtl->getTextureStage(i);
		HResource hTex = pStage->getTextureHandle();

		if (hTex == INVALID_RESOURCE_HANDLE || !pStage->getEnabled())
		{
			continue;
		}

		GfxTexture* pTex = AutoResPtr<GfxTexture>(hTex);

		if (!pTex)
		{
			continue;
		}

		// otherwise, insert a sampler
		source << "sampler";

		switch(pTex->getType())
		{
		case GfxTexture::TEX2D:
			source << "2D";
			samp2D = true;
			break;

		case GfxTexture::TEX3D:
			source << "3D";
			samp3D = true;
			break;

		case GfxTexture::TEXCUBE:
			source << "CUBE";
			sampCUBE = true;
			break;
		}

		source << " samp" << i << ";" << std::endl;
	}

	source << std::endl;

	// add sampler functions
	if (samp2D)
	{
		source << GfxShaderCode::ps[GfxShaderCode::PS_3_0][GfxShaderCode::PS_SAMPLE_2D];
	}

	if (samp3D)
	{
		source << GfxShaderCode::ps[GfxShaderCode::PS_3_0][GfxShaderCode::PS_SAMPLE_3D];
	}

	if (sampCUBE)
	{
		source << GfxShaderCode::ps[GfxShaderCode::PS_3_0][GfxShaderCode::PS_SAMPLE_CUBE];
	}
}
//---------------------------------------------------------------------------
static void addTextureSampling(
	GfxMaterial* pMtl, std::stringstream& source)
{
	for (size_t i=0; i<pMtl->getNumTextureStages(); ++i)
	{
		// otherwise, insert a sampler call
		GfxTextureStage* pStage = pMtl->getTextureStage(i);
		HResource hTex = pStage->getTextureHandle();
		if (hTex == INVALID_RESOURCE_HANDLE || !pStage->getEnabled())
		{
			continue;
		}

		GfxTexture* pTex = AutoResPtr<GfxTexture>(hTex);

		if (!pTex)
		{
			continue;
		}

		source << "\toCol ";

		// blend first stage with current color
		if (!i)
		{
			source << "*= ";
		}
		else
		{
			switch(pStage->getBlendMode())
			{
			case GfxTextureStage::BLENDMODE_ADD:
				source << "+= ";
				break;

			case GfxTextureStage::BLENDMODE_MODULATE:
				source << "*= ";
				break;

			case GfxTextureStage::BLENDMODE_REPLACE:
				source << "= ";
				break;
			}
		}

		source << "tex";

		switch(pTex->getType())
		{
		case GfxTexture::TEX3D:
			source << "3D";
			break;

		case GfxTexture::TEXCUBE:
			source << "CUBE";
			break;

		case GfxTexture::TEX2D:
		default:
			source << "2D";
			break;
		}

		source << "(samp" << i << ", psIN.tex";

		source << pStage->getTexCoordSet();

		source << ");" << std::endl;
	}
}
//---------------------------------------------------------------------------
static void addVertexColoring(
	GfxSubMesh* pSubMesh, 
	GfxMaterial* /*pMtl*/,
	std::stringstream& source)
{
	if (pSubMesh->getVertexFormat().hasColor())
	{
		source << "\toCol *= psIN.col0;" << std::endl;
	}
}
//---------------------------------------------------------------------------
static bool createPixelShaderSource(
	GfxSubMesh* pSubMesh, GfxMaterial* pMaterial, std::stringstream& source)
{
	//source.str("");
	source << GfxShaderCode::ps[GfxShaderCode::PS_3_0][GfxShaderCode::PS_GLOBALS];

	addSamplers(pMaterial, source);
	//source << GfxShaderCode::ps[GfxShaderCode::PS_3_0][GfxShaderCode::PS_IN_OPEN];
	//addVSOutputElements(pSubMesh, pMaterial, source);
	//source << GfxShaderCode::ps[GfxShaderCode::PS_3_0][GfxShaderCode::PS_IN_CLOSE];
	source << GfxShaderCode::ps[GfxShaderCode::PS_3_0][GfxShaderCode::PS_BLINN_LIGHTING];
	source << GfxShaderCode::ps[GfxShaderCode::PS_3_0][GfxShaderCode::PS_MAIN_OPEN];

	addVertexColoring(pSubMesh, pMaterial, source);

	// if we have textures, we need to sample and apply them; blend mode determines
	// how they are applied between layers
	addTextureSampling(pMaterial, source);

	if (pMaterial->getLit())
	{
		source << GfxShaderCode::ps[GfxShaderCode::PS_3_0][GfxShaderCode::PS_CALC_BLINN];
	}

	source << GfxShaderCode::ps[GfxShaderCode::PS_3_0][GfxShaderCode::PS_MAIN_CLOSE]; 
	source << std::endl;

	return true;
}
//---------------------------------------------------------------------------
void GfxShaderLib::dumpShaderSource(unsigned __int64 hash, const char *src)
{
	Environment& env(Environment::get());
	char buf[1024];
	env.pLogger->logMessage("****************************************");
	sprintf_s(buf, 1024, "Shader hash: %I64X\n", hash);
	env.pLogger->logMessage(buf);
	env.pLogger->logMessage(src);
	env.pLogger->logMessage("****************************************");
}
//---------------------------------------------------------------------------
bool loadCustomShaderSource(/*in*/const char* shaderName,
	/*out*/String& source, /*out*/String& fullpath)
{
	String name(shaderName);
	size_t pos = name.findFirst('/');
	String techniqueName = name.substr(pos+1);
	String shaderFile(name.substr(0, pos));
	shaderFile += ".fx";
	
	String path(Environment::get().pDataPath);
	path += "shaders/";
	path += shaderFile;
	fullpath = path;

	FileStream fs;
	if (fs.open(path, READ))
	{
		// hacky -- need to find a way to read it directly into the String
		char* buf = (char*)GetDEFAULTAllocator()->Allocate(fs.length()+1);
		fs.read(buf, fs.length());
		fs.close();
		buf[fs.length()] = 0;
		source = buf;
		GetDEFAULTAllocator()->Deallocate(buf);

		return true;
	}
	
	return false;
}
