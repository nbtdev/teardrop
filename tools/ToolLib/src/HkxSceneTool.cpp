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
#include "HkxSceneTool.h"

#include "Gfx/GfxRenderer.h"
#include "Gfx/GfxMesh.h"
#include "Gfx/GfxMaterial.h"
#include "Gfx/GfxSubMesh.h"
#include "Gfx/GfxIndexData.h"
#include "Gfx/GfxVertexData.h"
#include "Gfx/GfxCommon.h"
#include "Gfx/GfxUtil.h"
#include "Gfx/GfxTextureStage.h"
#include "Util/Environment.h"
#include "Util/Hash.h"
#include "Util/StringUtil.h"
#include "Stream/FileStream.h"
#include "Serialization/ResourceSerializer.h"
#include "tinyxml.h"
#include "Logger.h"
#include "SceneDataWriter.h"

#include "Common/Base/hkBase.h"
#include "Common/Serialize/Util/hkRootLevelContainer.h"
#include "Common/SceneData/Scene/hkxScene.h"
#include "Common/SceneData/Graph/hkxNode.h"
#include "Common/SceneData/Material/hkxMaterial.h"
#include "Common/SceneData/Material/hkxTextureFile.h"
#include "Common/SceneData/Mesh/hkxMesh.h"
#include "Common/SceneData/Mesh/hkxVertexBuffer.h"
#include "Common/SceneData/Mesh/hkxVertexDescription.h"
#include "Common/SceneData/Mesh/hkxIndexBuffer.h"
#include "Common/SceneData/Mesh/hkxMeshSection.h"
#include "Common/SceneData/Skin/hkxSkinBinding.h"
#include "Common/SceneData/Environment/hkxEnvironment.h"
#include "Common/SceneData/Attributes/hkxAttributeHolder.h"
#include "Common/SceneData/Attributes/hkxAttributeGroup.h"
#include "Physics/Utilities/Serialize/hkpPhysicsData.h"

#include <assert.h>
#include <stdio.h>
#include <map>
#include <list>
#include <string>
#include <sstream>

using namespace CoS;
//---------------------------------------------------------------------------
static char s_buf[8192];
static char strings[64 * 1024];
static char* s_pCurrentString = strings;
static const char* ToolName = "HkxSceneTool";
static Environment s_env;
typedef std::multimap<hkxMaterial*, hkxMeshSection*> MaterialToMeshLUT;
typedef std::list<hkxMeshSection*> MeshSections;
//---------------------------------------------------------------------------
//HKX_DT_NONE = 0,
//HKX_DT_UINT8, // only used for four contiguous hkUint8s, hkUint8[4]
//HKX_DT_INT16, // only used for old style quantized tcoords (0x7fff maps to 10.0f), so div by 3276.7f to get the float tcoords. Deprecated.
//HKX_DT_UINT32,
//HKX_DT_FLOAT,
//HKX_DT_FLOAT2, // for tex coords 
//HKX_DT_FLOAT3, // will always be 16byte aligned, so you can treat as a hkVector4 (with undefined w, with SIMD enabled etc)
//HKX_DT_FLOAT4  // will always be 16byte aligned, so you can treat as a hkVector4 (with SIMD enabled etc)
static size_t hkxSizeLut[] = 
{
	0,
	sizeof(hkUint8) * 4,
	sizeof(hkUint16),
	sizeof(hkUint32),
	sizeof(float),
	sizeof(float) * 2,
	sizeof(float) * 3,
	sizeof(hkVector4),
};

static CoS::VertexElementType elemTypeLut[] = 
{
	NOTSET,
	BYTE4,
	NOTSET,
	BYTE4,
	CoS::FLOAT,
	FLOAT2,
	FLOAT3,
	FLOAT4,
};
//---------------------------------------------------------------------------
HkxSceneTool::HkxSceneTool(const HkxSceneToolParams& params)
: m_params(params)
{
	m_pHkx = 0;
	m_numVerts = 0;
	m_numTris = 0;
	m_numMaterials = 0;
	m_numSubMeshes = 0;
	s_env.pRenderer = 0;
}
//---------------------------------------------------------------------------
HkxSceneTool::~HkxSceneTool()
{
}
//---------------------------------------------------------------------------
bool HkxSceneTool::initialize(hkRootLevelContainer* pHkx)
{
	m_pHkx = pHkx;
	m_pGfx = GfxRenderer::allocate(s_env);
	GfxRenderConfig cfg;
	//m_pGfx->initialize(cfg);
	s_env.pRenderer = m_pGfx;
	return true;
}
//---------------------------------------------------------------------------
bool HkxSceneTool::destroy()
{
	if (m_pGfx)
	{
		//m_pGfx->destroy();
	}

	GfxRenderer::deallocate(m_pGfx);
	m_pHkx = 0;
	m_numVerts = 0;
	m_numTris = 0;
	m_numMaterials = 0;
	m_numSubMeshes = 0;
	s_env.pRenderer = 0;
	return true;
}
//---------------------------------------------------------------------------
bool HkxSceneTool::process()
{
	if (!m_pHkx)
	{
		Logger::logMessage(Logger::ERR, ToolName, 
			"found null container, expected valid container");
		return false;
	}

	hkxScene* pScene = static_cast<hkxScene*>(m_pHkx->findObjectByType(
		hkxSceneClass.getName(), 0));

	if (!pScene)
	{
		Logger::logMessage(Logger::ERR, ToolName, 
			"no root scene node found, aborting");
		return false;
	}

	if (m_params.bVerbose)
	{
		Logger::logMessage(Logger::INFO, ToolName, 
			"found scene root, beginning processing");
	}

	if (!m_params.outDir)
	{
		Logger::logMessage(Logger::ERR, ToolName, 
			"no output directory provided, aborting");
		return false;
	}

	processScene(*pScene);

	if (m_params.bVerbose)
	{
		Logger::logMessage(Logger::INFO, ToolName, 
			"finished processing: %s", getStats());
	}

	return true;
}
//---------------------------------------------------------------------------
static void assembleVertexFormat(const hkxVertexDescription& rFmt, GfxVertexFormat& fmt)
{
	size_t offset = 0;
	size_t texUsage = 0, colorUsage = 0;
	int decls = rFmt.m_decls.getSize();

	for (int i=0; i<decls; ++i)
	{
		const hkxVertexDescription::ElementDecl* pDecl = 
			rFmt.getElementDeclByIndex(i);

		switch(pDecl->m_usage)
		{
		// vertex position
		case hkxVertexDescription::HKX_DU_POSITION:
			{
				GfxVertexFormat::Element elem;
				elem.semantic = POSITION;
				elem.type = elemTypeLut[pDecl->m_type];
				elem.offset = (unsigned char)offset;
				elem.setUsage(0);
				fmt.addElement(elem);

				offset += GfxUtil::getSizeOf((VertexElementType)elem.type);
			}
			break;

			// vertex normal
		case hkxVertexDescription::HKX_DU_NORMAL:
			{
				GfxVertexFormat::Element elem;
				elem.semantic = NORMAL;
				elem.type = elemTypeLut[pDecl->m_type];
				elem.offset = (unsigned char)offset;
				elem.setUsage(0);
				fmt.addElement(elem);

				offset += GfxUtil::getSizeOf((VertexElementType)elem.type);
			}
			break;

			// texture coords, if any
		case hkxVertexDescription::HKX_DU_TEXCOORD:
			{
				GfxVertexFormat::Element elem;
				elem.semantic = TEXCOORD;
				elem.type = elemTypeLut[pDecl->m_type];
				elem.offset = (unsigned char)offset;
				elem.setUsage((unsigned char)texUsage++);
				fmt.addElement(elem);

				offset += GfxUtil::getSizeOf((VertexElementType)elem.type);
			}
			break;

			// vertex color (diffuse only for now)
		case hkxVertexDescription::HKX_DU_COLOR:
			{
				GfxVertexFormat::Element elem;
				elem.semantic = COLOR;
				elem.type = elemTypeLut[pDecl->m_type];
				elem.offset = (unsigned char)offset;
				elem.setUsage((unsigned char)colorUsage++);
				fmt.addElement(elem);

				offset += GfxUtil::getSizeOf((VertexElementType)elem.type);
			}
			break;

			// vertex texture-space tangent
		case hkxVertexDescription::HKX_DU_TANGENT:
			{
				GfxVertexFormat::Element elem;
				elem.semantic = TANGENT;
				elem.type = elemTypeLut[pDecl->m_type];
				elem.offset = (unsigned char)offset;
				elem.setUsage(0);
				fmt.addElement(elem);

				offset += GfxUtil::getSizeOf((VertexElementType)elem.type);
			}
			break;

			// vertex binormal
		case hkxVertexDescription::HKX_DU_BINORMAL:
			{
				GfxVertexFormat::Element elem;
				elem.semantic = BINORMAL;
				elem.type = elemTypeLut[pDecl->m_type];
				elem.offset = (unsigned char)offset;
				elem.setUsage(0);
				fmt.addElement(elem);

				offset += GfxUtil::getSizeOf((VertexElementType)elem.type);
			}
			break;

			// skinning blend weight (if any)
		case hkxVertexDescription::HKX_DU_BLENDWEIGHTS:
			{
				GfxVertexFormat::Element elem;
				elem.semantic = BLENDWEIGHT;
				elem.type = elemTypeLut[pDecl->m_type];
				elem.offset = (unsigned char)offset;
				elem.setUsage(0);
				fmt.addElement(elem);

				offset += GfxUtil::getSizeOf((VertexElementType)elem.type);
			}
			break;

			// skinning blend index (if any)
		case hkxVertexDescription::HKX_DU_BLENDINDICES:
			{
				GfxVertexFormat::Element elem;
				elem.semantic = BLENDINDEX;
				elem.type = elemTypeLut[pDecl->m_type];
				elem.offset = (unsigned char)offset;
				elem.setUsage(0);
				fmt.addElement(elem);

				offset += GfxUtil::getSizeOf((VertexElementType)elem.type);
			}
			break;
		}
	}

	fmt.alignSize(16);
}
//---------------------------------------------------------------------------
static void addTextureStage(GfxMaterial* pMtl, TiXmlElement* pTex)
{
	if (!pTex)
	{
		return;
	}

	GfxTextureStage* pStage = pMtl->addTextureStage();
	pStage->setEnabled(true);
	TiXmlElement* pPath = pTex->FirstChildElement("fullpath");
	if (pPath)
	{
		const char* path = pPath->GetText();
		size_t len = strlen(path);
		memcpy(s_pCurrentString, path, len+1);
		pStage->setTextureName(s_pCurrentString);
		s_pCurrentString += len+1;
		s_pCurrentString = (char*)((size_t)(s_pCurrentString+4) & ~0x3); // align on 4-byte boundaries
	}

	unsigned int set = 0;
	const char* pSet = pTex->Attribute("set");
	if (pSet)
	{
		StringUtil::fromString(pSet, set);
		pStage->setTexCoordSet(set);
	}

	// layer blend type
	GfxTextureStage::BlendMode blend = GfxTextureStage::MODULATE;
	TiXmlElement* pBlend = pTex->FirstChildElement("blend");
	if (pBlend)
	{
		const char* pType = pBlend->Attribute("type");
		if (std::string("replace") == pType)
		{
			blend = GfxTextureStage::REPLACE;
		}
		if (std::string("add") == pType)
		{
			blend = GfxTextureStage::ADD;
		}
	}
	pStage->setLayerBlendMode(blend);

	// addressing modes
	GfxTextureStage::AddressMode u, v, w;
	u = v = w = GfxTextureStage::WRAP;

	TiXmlElement* pAddr = pTex->FirstChildElement("addressing");
	if (pAddr)
	{
		const char* pDir = pAddr->Attribute("u");
		if (pDir && std::string("mirror") == pDir)
		{
			u = GfxTextureStage::MIRROR;
		}
		if (pDir && std::string("clamp") == pDir)
		{
			u = GfxTextureStage::CLAMP;
		}

		pDir = pAddr->Attribute("v");
		if (pDir && std::string("mirror") == pDir)
		{
			v = GfxTextureStage::MIRROR;
		}
		if (pDir && std::string("clamp") == pDir)
		{
			v = GfxTextureStage::CLAMP;
		}

		pDir = pAddr->Attribute("w");
		if (pDir && std::string("mirror") == pDir)
		{
			w = GfxTextureStage::MIRROR;
		}
		if (pDir && std::string("clamp") == pDir)
		{
			w = GfxTextureStage::CLAMP;
		}

		// todo: extract the texture matrix info
	}

	pStage->setTextureAddressing(u, v, w);
}
//---------------------------------------------------------------------------
static void createMaterialFromXml(GfxMaterial* pMtl, const char* pXML)
{
	if (!pXML)
	{
		return;
	}

	TiXmlDocument doc;
	doc.Parse(pXML);
	if (doc.Error())
	{
		Logger::logMessage(Logger::WARNING, ToolName,
			"could not parse material xml");
		return;
	}

	TiXmlElement* pRoot = doc.RootElement();
	TiXmlElement* pShader = pRoot->FirstChildElement();

	// extract basic color information first
	if (!pShader)
	{
		Logger::logMessage(Logger::WARNING, ToolName,
			"malformed material XML found");
		return;
	}

	Vector4 col;
	TiXmlElement* pCol = pShader->FirstChildElement("diffuse");
	if (pCol)
	{
		StringUtil::fromString(pCol->Attribute("r"), col.x);
		StringUtil::fromString(pCol->Attribute("g"), col.y);
		StringUtil::fromString(pCol->Attribute("b"), col.z);
		StringUtil::fromString(pCol->Attribute("a"), col.w);
		pMtl->setDiffuse(GfxUtil::makePackedColor(col));
	}

	pCol = pShader->FirstChildElement("ambient");
	if (pCol)
	{
		StringUtil::fromString(pCol->Attribute("r"), col.x);
		StringUtil::fromString(pCol->Attribute("g"), col.y);
		StringUtil::fromString(pCol->Attribute("b"), col.z);
		StringUtil::fromString(pCol->Attribute("a"), col.w);
		pMtl->setAmbient(GfxUtil::makePackedColor(col));
	}

	pCol = pShader->FirstChildElement("emissive");
	if (pCol)
	{
		StringUtil::fromString(pCol->Attribute("r"), col.x);
		StringUtil::fromString(pCol->Attribute("g"), col.y);
		StringUtil::fromString(pCol->Attribute("b"), col.z);
		StringUtil::fromString(pCol->Attribute("a"), col.w);
		pMtl->setEmissive(GfxUtil::makePackedColor(col));
	}

	pCol = pShader->FirstChildElement("specular"); // only in blinn and phong
	if (pCol)
	{
		StringUtil::fromString(pCol->Attribute("r"), col.x);
		StringUtil::fromString(pCol->Attribute("g"), col.y);
		StringUtil::fromString(pCol->Attribute("b"), col.z);

		const char* attr = pCol->Attribute("power"); // phong
		if (!attr)
		{
			attr = pCol->Attribute("eccentricity"); // blinn
		}
		StringUtil::fromString(attr, col.w);

		pMtl->setSpecular(GfxUtil::makePackedColor(col));
	}

	pMtl->setNumLights(1); // todo: get this from somewhere?
	//if (std::string("lambert") == pShader->Value())
	//	pMtl->setShadingModel(GfxMaterial::LAMBERT);
	//else if (std::string("phong") == pShader->Value())
	//	pMtl->setShadingModel(GfxMaterial::PHONG);
	//else if (std::string("blinn") == pShader->Value())
	//	pMtl->setShadingModel(GfxMaterial::BLINN);
	//else
	//{
	//	pMtl->setShadingModel(GfxMaterial::UNLIT);
	//	pMtl->setNumLights(0);
	//}

	pMtl->setDepthCheck(true);
	if (pShader->Attribute("transparent"))
	{
		pMtl->setAlphaBlended(true);
		pMtl->setDepthWrite(false);
	}
	else
	{
		pMtl->setAlphaBlended(false);
		pMtl->setDepthWrite(true);
	}

	// add textures as needed
	TiXmlElement* pTex = pRoot->FirstChildElement("texture");
	while (pTex)
	{
		addTextureStage(pMtl, pTex);
		pTex = pTex->NextSiblingElement("texture");
	}
}
//---------------------------------------------------------------------------
static GfxMaterial* makeMaterial(const hkxMaterial* pMtl)
{
	void* pMem = GFX_ALLOCATE(sizeof(GfxMaterial));
	memset(pMem, 0, sizeof(GfxMaterial));
	GfxMaterial* pMaterial = ::new(pMem) GfxMaterial(0);
	pMaterial->initialize();

	void* pArcMtl = 
		pMtl->findAttributeObjectByName("ArcadiaMaterial");
	hkxSparselyAnimatedString* pXmlAttr = 
		static_cast<hkxSparselyAnimatedString*>(pArcMtl);
	const char* pXML = pXmlAttr->m_strings[0];
	if (pXML)
	{
		createMaterialFromXml(pMaterial, pXML);
	}

#if 0
	// set material properties from the hkx material
	pMaterial->setDiffuse(GfxUtil::makePackedColor(
		(Vector4&)pMtl->m_diffuseColor));
	pMaterial->setAmbient(GfxUtil::makePackedColor(
		(Vector4&)pMtl->m_ambientColor));
	pMaterial->setSpecular(GfxUtil::makePackedColor(
		pMtl->m_specularColor(0),
		pMtl->m_specularColor(1),
		pMtl->m_specularColor(2),
		0)); // w is specular power
	pMaterial->setEmissive(GfxUtil::makePackedColor(
		(Vector4&)pMtl->m_emissiveColor));

	for (int i=0; i<pMtl->m_numStages; ++i)
	{
		hkxMaterial::TextureStage& pStage = pMtl->m_stages[i];
		if (pStage.m_texture.m_class->getName() == "hkxTextureFile")
		{
			hkxTextureFile* pTexFile = 
				static_cast<hkxTextureFile*>(pStage.m_texture.m_object);

			GfxTextureStage* pGfxStage = pMaterial->addTextureStage();
			pGfxStage->setEnabled(true);
			pGfxStage->setTexCoordSet(pStage.m_tcoordChannel);
			pGfxStage->setTextureName(pTexFile->m_filename);

			// set map hint
			GfxTextureStage::MapHint hint = GfxTextureStage::MAP_UNKNOWN;
			switch(pStage.m_usageHint)
			{
			case hkxMaterial::TEX_DIFFUSE:
				hint = GfxTextureStage::MAP_DIFFUSE;
				break;

			case hkxMaterial::TEX_REFLECTION:
				hint = GfxTextureStage::MAP_ENVIRONMENT;
				break;

			case hkxMaterial::TEX_BUMP:
				hint = GfxTextureStage::MAP_NORMAL;
				break;

			case hkxMaterial::TEX_NORMAL:
				hint = GfxTextureStage::MAP_NORMAL;
				break;

			case hkxMaterial::TEX_DISPLACEMENT:
				hint = GfxTextureStage::MAP_DISPLACEMENT;
				break;

				// todo: if we figure out how to insert custom
				// map hints, extract them here
			}

			pGfxStage->setMapHint(hint);
		}
	}
#endif

	return pMaterial;
}
//---------------------------------------------------------------------------
static void makeSubMeshes(
	const MeshSections& sections, 
	GfxMesh& destMesh
)
{
	if (!sections.size())
	{
		Logger::logMessage(Logger::WARNING, ToolName,
			"mesh with no source mesh sections found, ignoring");
		return;
	}

	// find out how many total verts
	size_t vertCount = 0;
	size_t indexCount = 0;
	MeshSections filtered;
	for (std::list<hkxMeshSection*>::const_iterator it = sections.begin();
		it != sections.end(); ++it)
	{
		hkxMeshSection* pSect = *it;

		if (pSect->m_indexBuffers.getSize() > 1)
		{
			Logger::logMessage(Logger::WARNING, ToolName,
				"mesh section with more than one index buffer found, ignoring");
			continue;
		}

		hkxIndexBuffer* pIB = pSect->m_indexBuffers[0];
		if (pIB->m_indices32.getSize())
		{
			Logger::logMessage(Logger::WARNING, ToolName,
				"mesh section with 32-bit index buffer found, ignoring");
			continue;
		}

		filtered.push_back(*it);

		vertCount += pSect->m_vertexBuffer->getNumVertices();
		indexCount += pIB->m_indices16.getSize();
	}

	for (MeshSections::iterator it = filtered.begin(); it != filtered.end(); ++it)
	{
		// figure out the vertex format
		GfxVertexFormat fmt;
		size_t bonesPerVertex = 0;
		size_t numTextures = 0;

		const hkxMeshSection* pSect = *it;
		const hkxVertexDescription& rFmt = pSect->m_vertexBuffer->getVertexDesc();
		assembleVertexFormat(rFmt, fmt);

		// create a new submesh for these sections
		GfxSubMesh* pSubMesh = destMesh.createSubMesh();
		pSubMesh->setVertexFormat(s_env, fmt);

		// make a vertex data block with enough room for all section verts
		size_t streamIdx;
		const hkxVertexDescription::ElementDecl* posDecl = 
			rFmt.getElementDecl(hkxVertexDescription::HKX_DU_POSITION, 0);
		GfxVertexData* pVertData = pSubMesh->createVertexData(
			streamIdx,
			s_env,
			//fmt.getVertexSize(),
			posDecl->m_byteStride,
			pSect->m_vertexBuffer->getNumVertices());
		pSubMesh->setPrimitiveType(TRILIST);
		void* pVertBuf = pVertData->getBuffer();

		// likewise for index data
		GfxIndexData* pIndexData = pSubMesh->createIndexData(
			s_env,
			sizeof(short), 
			pSect->m_indexBuffers[0]->m_indices16.getSize());

		void* pIndexBuf = pIndexData->getBuffer();

		// gather up all vert and index data into the new buffers
		char* pcV = (char*)pVertBuf;
		char* pcI = (char*)pIndexBuf;

		hkxIndexBuffer* pIB = pSect->m_indexBuffers[0];
		hkxVertexBuffer* pVB = pSect->m_vertexBuffer;

		if (posDecl->m_byteStride != fmt.getVertexSize())
		{
			// throw up a warning here?
			break;
		}

		memcpy(pcV, pVB->getVertexDataPtr(*posDecl), 
			posDecl->m_byteStride * pVB->getNumVertices());
		memcpy(pcI, pIB->m_indices16.begin(), pIB->m_indices16.getSize() * sizeof(short));

		// need to set all of the .w values to 1 (Havok sets them to zero which is bad for graphics)
		for (hkUint32 i=0; i<(hkUint32)pVB->getNumVertices(); ++i)
		{
			hkVector4* posVals = (hkVector4*)(pcV + i * posDecl->m_byteStride);
			(*posVals)(3) = 1;
		}


		pcV += pVB->getNumVertices() * posDecl->m_byteStride;
		pcI += pIB->m_indices16.getSize() * 2;

		//m_numVerts += pVB->getNumVertices();
		//m_numTris += (pIB->m_numIndices16 / 3);

		pIndexData->unlock();
		pVertData->unlock();

		// create material for this submesh
		pSubMesh->setMaterial(makeMaterial(pSect->m_material));

		//++m_numSubMeshes;
		//++m_numMaterials;
	}
}
//---------------------------------------------------------------------------
void HkxSceneTool::processScene(const hkxScene& pScene)
{
	size_t numNodes = pScene.m_rootNode->m_children.getSize();
	SceneEntries sceneEntries;

	std::string stem(m_params.outFile);
	stem = stem.substr(0, stem.find_last_of("\\/")+1);

	for (size_t i=0; i < numNodes; ++i)
	{
		s_pCurrentString = strings;
		GfxMesh destMesh;
		destMesh.initialize();

		hkxNode* pNode = pScene.m_rootNode->m_children[i];

		hkVariant v = pNode->m_object;
		hkxMesh* pMesh = 0;
		if (v.m_object)
		{
			if (v.m_class == &hkxMeshClass)
			{
				pMesh = static_cast<hkxMesh*>(v.m_object);
			}
		}

		if (pMesh)
		{
			// group the meshes by the material they use
			MeshSections sections;
			size_t sects = pMesh->m_sections.getSize();
			for (size_t i=0; i<sects; ++i)
			{
				hkxMeshSection* pSection = pMesh->m_sections[i];
				sections.push_back(pSection);
			}

			makeSubMeshes(sections, destMesh);

			// remove any offending characters from the name (Havok/Maya like to put : in the node names)
			char name[1024];
			char* p = name;
			strcpy_s(name, 1024, pNode->m_name);
			while (*p)
			{
				if (*p == ':' || *p == '/' || *p == '\\' || *p == '|')
				{
					*p = '_';
				}

				++p;
			}

			// open the output file for this mesh
			std::string outName(m_params.outDir);
			outName += "\\";
			outName += name;
			outName += ".msh";

			FileStream fs;
			if (!fs.open((stem+outName).c_str(), WRITE|BINARY|TRUNCATE))
			{
				std::stringstream ss;
				ss << "Could not open output file " << outName;
				Logger::logMessage(Logger::INFO, ToolName, ss.str().c_str());
				continue;
			}

			ResourceSerializer ser(fs);
			destMesh.serialize(ser);
			fs.close();
			destMesh.destroy();

			// gather up the node info for generating the scene file
			if (!pNode->m_keyFrames.getSize())
			{
				std::stringstream ss;
				ss << "Node " << pNode->m_name << " has no transform!";
				Logger::logMessage(Logger::INFO, ToolName, ss.str().c_str());
				continue;
			}

			SceneEntry ent;
			destMesh.getAABB((Vector4&)ent.minCorner, (Vector4&)ent.maxCorner);

			hkMatrix4& mat = pNode->m_keyFrames[0];
			hkTransform xform;
			mat.get(xform);
			hkRotation& rot = xform.getRotation();
			if (!rot.isOrthonormal())
			{
				ent.angle = 0;
				ent.axis[0] =
				ent.axis[1] =
				ent.axis[2] = 0;
			}
			else
			{
				hkQuaternion q(rot);
				ent.angle = q.getAngle();
				if (q.hasValidAxis())
				{
					q.getAxis((hkVector4&)ent.axis);
				}
				else
				{
					ent.axis[0] = 
					ent.axis[1] = 
					ent.axis[2] = 0;
				}
			}

			(hkVector4&)ent.trans = xform.getTranslation();
			(hkVector4&)ent.scale = hkVector4(1, 1, 1);
			ent.name = outName;

			sceneEntries.push_back(ent);
		}
	}

	// generate a scene document, if there was anything exported
	SceneDataWriter writer;
	writer.write(sceneEntries, m_params.outFile);

	//else
	//{
		//// do them each individually, as they appear in the export
		//for (std::list<hkxMesh*>::iterator it = meshList.begin(); 
		//	it != meshList.end(); ++it)
		//{
		//	hkxMesh* pMesh = *it;
		//	size_t sects = pMesh->m_numSections;
		//	for (size_t i=0; i<sects; ++i)
		//	{
		//		MeshSections sections;
		//		hkxMeshSection* pSection = pMesh->m_sections[i];
		//		sections.push_back(pSection);
		//		makeSubMesh(sections, destMesh);
		//	}
		//}
	//}
}
//---------------------------------------------------------------------------
const char* HkxSceneTool::getStats()
{
	sprintf_s(s_buf, 
		"\n\tVertices:        %d\n"
		"\tTris:            %d\n"
		"\tMaterials:       %d\n"
		"\tSubmeshes:       %d\n",
		m_numVerts,
		m_numTris,
		m_numMaterials,
		m_numSubMeshes);
	return s_buf;
}
