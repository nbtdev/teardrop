/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "HkxMeshTool.h"
#include "Gfx/GfxRenderer.h"
#include "Config.h"

#include "Common/Base/hkBase.h"
#include "Common/Serialize/Util/hkRootLevelContainer.h"
#include "Common/SceneData/Scene/hkxScene.h"
#include "Common/SceneData/Material/hkxMaterial.h"
#include "Common/SceneData/Material/hkxTextureFile.h"
#include "Common/SceneData/Mesh/hkxMesh.h"
#include "Animation/Animation/Deform/Skinning/hkaMeshBinding.h"
#include "Animation/Animation/hkaAnimationContainer.h"
#include "Common/SceneData/Mesh/hkxVertexBuffer.h"
#include "Common/SceneData/Mesh/hkxVertexDescription.h"
#include "Common/SceneData/Mesh/hkxIndexBuffer.h"
#include "Common/SceneData/Mesh/hkxMeshSection.h"
#include "Common/SceneData/Skin/hkxSkinBinding.h"
#include "Common/SceneData/Attributes/hkxAttributeHolder.h"
#include "Common/SceneData/Attributes/hkxAttributeGroup.h"
#include "Common/SceneData/Environment/hkxEnvironment.h"
#include "Physics/Utilities/Serialize/hkpPhysicsData.h"

#include "Gfx/GfxMesh.h"
#include "Gfx/GfxMaterial.h"
#include "Gfx/GfxSubMesh.h"
#include "Gfx/GfxIndexData.h"
#include "Gfx/GfxVertexData.h"
#include "Gfx/GfxCommon.h"
#include "Gfx/GfxUtil.h"
#include "Gfx/GfxTextureStage.h"
#include "Util/Environment.h"
#include "Util/StringUtil.h"
#include "Util/Hash.h"
#include "Math/MathUtil.h"
#include "Math/Matrix44.h"

#include "Logger.h"
#include "TinyXML/tinyxml.h"

#include <assert.h>
#include <stdio.h>
#include <map>
#include <list>
#include <string>

using namespace Teardrop;
//---------------------------------------------------------------------------
static char s_buf[8192];
static char strings[64 * 1024];
static char* s_pCurrentString = strings;
static const char* ToolName = "HkxMeshTool";
static Environment s_env;
class Pair
{
public:
	hkxMesh* pMesh;
	hkaMeshBinding* pBinding;
	Pair(hkxMesh* m) { pMesh =m; pBinding =0; }
	Pair(hkaMeshBinding* b) { pMesh =0; pBinding =b; }
private:
	Pair();
};
class Pair2
{
public:
	hkxMeshSection* pSect;
	hkaMeshBinding* pBinding;
	Pair2(hkxMeshSection* m) { pSect =m; pBinding =0; }
	Pair2(hkaMeshBinding* b) { pSect =0; pBinding =b; }
	Pair2(hkxMeshSection* m, hkaMeshBinding* b) { pSect =m; pBinding =b; }
private:
	Pair2();
};
typedef std::multimap<hkxMaterial*, Pair2> MaterialToMeshLUT;
typedef std::list<Pair2> MeshSections;
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
	sizeof(float) * 4,
	sizeof(hkVector4),
};

static Teardrop::VertexElementType elemTypeLut[] = 
{
	NOTSET,
	BYTE4,
	NOTSET,
	BYTE4,
	Teardrop::FLOAT,
	FLOAT2,
	FLOAT4,
	FLOAT4,
};
//---------------------------------------------------------------------------
HkxMeshTool::HkxMeshTool(const HkxMeshToolParams& params)
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
HkxMeshTool::~HkxMeshTool()
{
}
//---------------------------------------------------------------------------
bool HkxMeshTool::initialize(hkRootLevelContainer* pHkx)
{
	m_pHkx = pHkx;
	m_pGfx = GfxRenderer::allocate(s_env);
	GfxRenderConfig cfg;
	//m_pGfx->initialize(cfg);
	s_env.pRenderer = m_pGfx;
	return true;
}
//---------------------------------------------------------------------------
bool HkxMeshTool::destroy()
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
bool HkxMeshTool::process(
	GfxMesh& destMesh, hkRootLevelContainer& destContainer)
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

	_process(destMesh);

	if (m_params.bVerbose)
	{
		Logger::logMessage(Logger::INFO, ToolName, 
			"finished processing: %s", getStats());
	}

	return true;
}
//---------------------------------------------------------------------------
bool HkxMeshTool::process(GfxMesh& destMesh)
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

	_process(destMesh);

	return true;
}
//---------------------------------------------------------------------------
void assembleVertexFormat(const hkxVertexDescription& rFmt, GfxVertexFormat& fmt)
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

	pMtl->setNumLights(4); // todo: get this from somewhere?
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

	// TODO: force use of custom shader for anything exported? Hmm...
	pMtl->setCustomShader(GfxMaterial::SHADER_FORCE_SHORT);

}
//---------------------------------------------------------------------------
static GfxMaterial* makeMaterial(const hkxMaterial* pMtl)
{
	GfxMaterial* pMaterial = TD_NEW GfxMaterial;
	pMaterial->initialize();

	void* pArcMtl = 
		pMtl->findAttributeObjectByName("CoSMaterial");
	if (pArcMtl) 
	{
		hkxSparselyAnimatedString* pXmlAttr = 
			static_cast<hkxSparselyAnimatedString*>(pArcMtl);
		const char* pXML = pXmlAttr->m_strings[0];

		if (pXML)
		{
			createMaterialFromXml(pMaterial, pXML);
		}
	}
	else
	{
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

		for (int i=0; i<pMtl->m_stages.getSize(); ++i)
		{
			const hkxMaterial::TextureStage& pStage = pMtl->m_stages[i];
			if (pStage.m_texture.getClass()->getName() == "hkxTextureFile")
			{
				hkxTextureFile& texFile = 
					static_cast<hkxTextureFile&>(*(pStage.m_texture));

				GfxTextureStage* pGfxStage = pMaterial->addTextureStage();
				pGfxStage->setEnabled(true);
				pGfxStage->setTexCoordSet(pStage.m_tcoordChannel);
				pGfxStage->setTextureName(texFile.m_filename);

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

		// TODO: force use of custom shader for anything exported? Hmm...
		pMaterial->setCustomShader(GfxMaterial::SHADER_FORCE_SHORT);
	}

	return pMaterial;
}
//---------------------------------------------------------------------------
static void makeSubMesh(
	const MeshSections& sections, 
	GfxMesh& destMesh
)
{
	if (!sections.size())
	{
		Logger::logMessage(Logger::WARNING, ToolName,
			"submesh with no source mesh sections found, ignoring");
		return;
	}

	// find out how many total verts
	size_t vertCount = 0;
	size_t indexCount = 0;
	int boneCount = 0;
	for (MeshSections::const_iterator it = sections.begin();
		it != sections.end(); ++it)
	{
		hkxMeshSection* pSect = it->pSect;
		hkaMeshBinding* pBinding = it->pBinding;

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

		vertCount += pSect->m_vertexBuffer->getNumVertices();
		indexCount += pIB->m_indices16.getSize();

		if (pBinding)
			boneCount = MathUtil::max(boneCount, pBinding->m_boneFromSkinMeshTransforms.getSize());
	}


	// figure out the vertex format
	GfxVertexFormat fmt;
	size_t bonesPerVertex = 0;
	size_t numTextures = 0;

	const hkxMeshSection* pFirst = sections.begin()->pSect;
	hkxVertexBuffer* pHkVB = pFirst->m_vertexBuffer;
	const hkxVertexDescription& rFmt = pHkVB->getVertexDesc();
	assembleVertexFormat(rFmt, fmt);

	// create a new submesh for these sections
	GfxSubMesh* pSubMesh = destMesh.createSubMesh();
	pSubMesh->setVertexFormat(s_env, fmt);

	// init some storage for all of the bind pose information for this submesh
	pSubMesh->getBindPoses().resize(boneCount);

	// make a vertex data block with enough room for all section verts
	size_t streamIdx;
	const hkxVertexDescription::ElementDecl* posDecl = 
		rFmt.getElementDecl(hkxVertexDescription::HKX_DU_POSITION, 0);
	GfxVertexData* pVertData = pSubMesh->createVertexData(
		streamIdx,
		s_env,
		fmt.getVertexSize(),
		vertCount,
		GfxVertexData::CreationFlags(GfxVertexData::STATIC | GfxVertexData::WRITE_ONLY));
	pSubMesh->setPrimitiveType(TRILIST);
	void* pVertBuf = pVertData->getBuffer();

	// likewise for index data
	GfxIndexData* pIndexData = pSubMesh->createIndexData(
		s_env,
		sizeof(short), 
		indexCount);

	void* pIndexBuf = pIndexData->getBuffer();

	// gather up all vert and index data into the new buffers
	char* pcV = (char*)pVertBuf;
	short* pcI = (short*)pIndexBuf;
	int offset = 0;

	hkaMeshBinding* pMasterBinding = 0;
	for (MeshSections::const_iterator it = sections.begin();
		it != sections.end(); ++it)
	{
		hkxMeshSection* pSect = it->pSect;
		hkaMeshBinding* pBinding = it->pBinding;
		if (!pMasterBinding)
		{
			pMasterBinding = pBinding;
		}

		if (pSect->m_indexBuffers.getSize() > 1)
		{
			continue;
		}

		hkxIndexBuffer* pIB = pSect->m_indexBuffers[0];
		if (pIB->m_indices32.getSize())
		{
			continue;
		}

		hkxVertexBuffer* pVB = pSect->m_vertexBuffer;

		// so as of HK 7.x, we can no longer just copy vertex data directly,
		// we basically have to parse it out of their new data layout (which 
		// is stored by type, i.e. they have an array of hkVector4, an array
		// of float, an array of uint8 and so on). We want to copy this out in
		// an order that matches the vertex declaration we've set up for
		// our GfxMesh vertex buffers, so since we created our vert decl based
		// on theirs (in the same order), we can use theirs as the master for
		// this operation. 

		for (int vert=0; vert<pVB->getNumVertices(); ++vert)
		{
			for (int i=0; i<rFmt.m_decls.getSize(); ++i)
			{
				const hkxVertexDescription::ElementDecl& rDecl = rFmt.m_decls[i];
				int elemSize = hkxSizeLut[rDecl.m_type];
				char* srcPtr = (char*)pVB->getVertexDataPtr(rDecl);
				int stride = rDecl.m_byteStride;

				// copy the bytes into the current "cursor" position...
				memcpy(pcV, srcPtr + stride*vert, elemSize);

				// and still to this day we need to fix Havok's vertex position
				// .w component...
				if (rDecl.m_usage == hkxVertexDescription::HKX_DU_POSITION)
				{
					Vector4* pVec = (Vector4*)pcV;
					pVec->w = 1;
				}

				// and then advance our "cursor"
				pcV += elemSize;
			}
		}

		// indices have to be copied piecewise because they have to take
		// into account the additional offsets of the verts they index
		for (int i=0; i<pIB->m_indices16.getSize(); ++i)
		{
			*(pcI++) = pIB->m_indices16[i] + offset;
		}

		offset += pVB->getNumVertices();
	}

	// and then, copy over the bind pose data for skinning (if any)
	if (pMasterBinding && pMasterBinding->m_boneFromSkinMeshTransforms.getSize())
	{
		for (int i=0; i<pMasterBinding->m_boneFromSkinMeshTransforms.getSize(); ++i)
		{
			pSubMesh->getBindPoses()[i] = 
				&((Matrix44&)pMasterBinding->m_boneFromSkinMeshTransforms[i]);
		}
	}

	// create material for this submesh
	pSubMesh->setMaterial(makeMaterial(pFirst->m_material));
}
//---------------------------------------------------------------------------
void mergeMeshes(const MaterialToMeshLUT& lut, GfxMesh& destMesh)
{
	// go through each group (multimap "first" values) and make a single 
	// (potentially skinned) mesh of each
	hkxMaterial* pPrevMtl = 0;
	MeshSections sectionList;
	MaterialToMeshLUT::const_iterator it = lut.begin();
	size_t idx = 0;

	for (; it != lut.end(); ++it) 
	{
		hkxMaterial* pMtl = it->first;
		if (!pMtl)
		{
			Logger::logMessage(Logger::WARNING, ToolName,
				"mesh with null material found, ignoring");
			continue;
		}

		if (pMtl != pPrevMtl)
		{
			// merge these into a single SubMesh in the dest GfxMesh
			if (sectionList.size())
			{
				makeSubMesh(sectionList, destMesh);
			}

			pPrevMtl = pMtl;
			sectionList.clear();
		}

		sectionList.push_back(Pair2(it->second.pSect, it->second.pBinding));
	}

	makeSubMesh(sectionList, destMesh);
}
//---------------------------------------------------------------------------
void HkxMeshTool::_process(GfxMesh& destMesh)
{
	/**
		The point is to go through the top-level of the scene and find 
		meshes -- we check for skins first because we only want to grab the
		meshes that are animated if the object is animated, and if it is not
		animated, then we grab all static meshes. We then have a set of meshes
		that need to be grouped together by material, at which point they will
		be merged into one-mesh-per-material groups. 
	*/

	hkxScene* pScene = reinterpret_cast<hkxScene*>(
		m_pHkx->findObjectByType(hkxSceneClass.getName()));

	size_t numSkins = pScene->m_skinBindings.getSize();
	size_t numMeshes = pScene->m_meshes.getSize();
	std::list<Pair> meshList;

	if (numSkins)
	{
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>(
			m_pHkx->findObjectByType(hkaAnimationContainerClass.getName()));

		m_params.bVerbose ? Logger::logMessage(Logger::INFO, ToolName, 
			"found skins, meshes will be treated as animated") : 0;

		for (size_t i=0; i < numSkins; ++i)
		{
			hkaMeshBinding* pBinding = ac->m_skins[i];
			meshList.push_back(Pair(pBinding));
		}
	}
	else
	{
		m_params.bVerbose ? Logger::logMessage(Logger::INFO, ToolName, 
			"no skins found, meshes will be treated as static") : 0;

		for (size_t i=0; i < numMeshes; ++i)
		{
			meshList.push_back(Pair(pScene->m_meshes[i]));
		}
	}

	if (m_params.bMergeMeshesByMaterial)
	{
		// group the meshes by the material they use
		MaterialToMeshLUT materialMeshLUT;
		for (std::list<Pair>::iterator it = meshList.begin(); 
			it != meshList.end(); ++it)
		{
			hkxMesh* pMesh;
			if (it->pMesh)
			{
				pMesh = it->pMesh;
			}
			else
			{
				pMesh = it->pBinding->m_mesh;
			}

			size_t sects = pMesh->m_sections.getSize();
			for (size_t i=0; i<sects; ++i)
			{
				hkxMeshSection* pSection = pMesh->m_sections[i];
				MaterialToMeshLUT::value_type val(
					pSection->m_material, 
					Pair2(pMesh->m_sections[i], it->pBinding));

				materialMeshLUT.insert(val);
			}
		}

		mergeMeshes(materialMeshLUT, destMesh);
	}
	else
	{
		// do them each individually, as they appear in the export
		for (std::list<Pair>::iterator it = meshList.begin(); 
			it != meshList.end(); ++it)
		{
			hkxMesh* pMesh;
			if (it->pMesh)
			{
				pMesh = it->pMesh;
			}
			else
			{
				pMesh = it->pBinding->m_mesh;
			}

			size_t sects = pMesh->m_sections.getSize();
			for (size_t i=0; i<sects; ++i)
			{
				MeshSections sections;
				hkxMeshSection* pSection = pMesh->m_sections[i];
				if (!pSection->m_material)
				{
					Logger::logMessage(Logger::WARNING, ToolName,
						"mesh with null material found, ignoring");
					continue;
				}

				sections.push_back(Pair2(pSection, it->pBinding));
				makeSubMesh(sections, destMesh);
			}
		}
	}
}
//---------------------------------------------------------------------------
const char* HkxMeshTool::getStats()
{
	sprintf_s(s_buf, 
		"\tVertices:        %d\n"
		"\tTris:            %d\n"
		"\tMaterials:       %d\n"
		"\tSubmeshes:       %d\n",
		m_numVerts,
		m_numTris,
		m_numMaterials,
		m_numSubMeshes);
	return s_buf;
}
