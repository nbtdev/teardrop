/******************************************************************************
Copyright (c) 2015 Teardrop Games

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
******************************************************************************/

#include "AssetImport.h"
#include "AssetImportException.h"
#include "Metadata.h"
#include "ImportedAsset.h"
#include "Asset/StaticMeshAsset.h"
#include "Asset/TextureAsset.h"
#include "Gfx/Exception.h"
#include "Gfx/Material.h"
#include "Gfx/MaterialOutput.h"
#include "Gfx/Sampler2DExpression.h"
#include "Gfx/Connection.h"
#include "Gfx/Attribute.h"
#include "Gfx/IndexBuffer.h"
#include "Gfx/VertexBuffer.h"
#include "Gfx/VertexElement.h"
#include "Gfx/Submesh.h"
#include "Gfx/Mesh.h"
#include "Math/Vector4.h"
#include "Math/Vector2.h"
#include "Util/UUID.h"
#include "Util/_String.h"
#include "fbxsdk.h"
#include <map>
#include <memory>

using namespace fbxsdk_2015_1;

namespace Teardrop {
	using namespace Gfx;

	namespace Tools {

		typedef std::map<String, Gfx::Material*> MaterialTable;

		static Gfx::Material* importMaterial(FbxSurfaceMaterial* mtl, ImportedAsset& imp)
		{
			MaterialOutput* output = TD_NEW MaterialOutput;
			UUID uuid;
			uuid.generate();
			output->setObjectId(uuid);
			output->initialize();
			imp.addInternalDep(output);

			Material* gfxMtl = TD_NEW Material;
			gfxMtl->setOutput(output);

			// check for diffuse texture, add if present
			FbxProperty prop = mtl->FindProperty(FbxSurfaceMaterial::sDiffuse);
			int nObj = prop.GetSrcObjectCount<FbxTexture>();
			if (nObj) {
				for (int i=0; i<nObj; ++i) {
					FbxTexture* tex = prop.GetSrcObject<FbxTexture>(i);
					if (tex) {
						const char* texName = (const char*) tex->GetName();
						int nFileTex = prop.GetSrcObjectCount<FbxFileTexture>();

						for (int f=0; f<nFileTex; ++f) {
							FbxFileTexture* fileTex = prop.GetSrcObject<FbxFileTexture>(f);
							if (fileTex) {
								String fileName((const char*) fileTex->GetFileName());

								TextureAsset* texAsset = importTexture(fileName, TEXTUREASSET_TYPE_BCX);

								if (texAsset) {
									Sampler2DExpression* expr = TD_NEW Sampler2DExpression;
									uuid.generate();
									expr->setObjectId(uuid);
									expr->initialize();
									expr->getSampler2D().setTextureAsset(texAsset);
									imp.addInternalDep(expr);
									imp.addDep(texAsset, texName, fileName);

									Connection* conn = TD_NEW Connection;
									conn->setFromExpression(expr); conn->setFromAttribute("Color");
									conn->setToExpression(output); conn->setToAttribute("Diffuse");
									conn->setParent(gfxMtl);
									uuid.generate();
									conn->setObjectId(uuid);
									conn->initialize();
									imp.addInternalDep(conn);
								}
							}
						}
					}
				}
			}

			return gfxMtl;
		}

		static bool importMesh(FbxNode* node, StaticMeshAsset* asset, ImportedAsset& imp, MaterialTable& table, const char* filepath)
		{
			// then the mesh data
			FbxMesh* fbxMesh = (FbxMesh*)node->GetNodeAttribute();
			Gfx::Mesh* gfxMesh = asset->mesh();

			int nPoly = fbxMesh->GetPolygonCount();
			// note: assets must be triangulated, so check for polygons with exactly 3 edges
			for (int i=0; i<nPoly; ++i) {
				if (fbxMesh->GetPolygonSize(i) != 3) {
					throw InvalidAssetFormatException(std::string(filepath), "Mesh is not triangulated");
				}
			}

			// then import the polys for real, starting with indices
			int* indices = fbxMesh->GetPolygonVertices();
			int nIndices = nPoly * 3;
			Gfx::Submesh* submesh = gfxMesh->createSubmesh();

			// if the number of indices is less than 65536, we need to copy the data (which is 4-byte integers) into
			// a tmp buffer of shorts, and use that to source the index data (the IB initialize method will look
			// simply at the number of indices and set the index size to 2 or 4 on that basis)
			std::unique_ptr<uint16_t> tmpIB;
			void* ibData = indices;
			if (nIndices < 65536) {
				uint16_t* tmp = new uint16_t[nIndices];
				tmpIB.reset(tmp);
				ibData = tmp;

				for (int idx = 0; idx < nIndices; ++idx) {
					tmp[idx] = uint16_t(indices[idx]);
				}
			}

			// TODO: make sure this really is what the incoming mesh data is...
			submesh->setPrimitiveType(Submesh::PT_TRILIST);

			IndexBuffer* ib = submesh->createIndexBuffer();

			try {
				ib->initialize(nIndices, 0, ibData);
			} catch (const Gfx::Exception&) {
				return false;
			}

			// the vertex array is called "control points"
			FbxVector4* verts = fbxMesh->GetControlPoints();
			int nVerts = fbxMesh->GetControlPointsCount();

			// FbxVector4 is doubles...
			std::vector<Vector4> tmpPos(nVerts);
			for (int i=0; i<nVerts; ++i) {
				tmpPos[i].x = (float)verts[i][0];
				tmpPos[i].y = (float)verts[i][1];
				tmpPos[i].z = (float)verts[i][2];
				tmpPos[i].w = (float)verts[i][3];
			}

			// put each data stream in its own vertex buffer, starting with positions
			VertexBuffer* vbPos = submesh->createVertexBuffer();
			vbPos->beginAddVertexElements();
			VertexElement& elem = vbPos->addVertexElement();
			elem.mType = VET_FLOAT;
			elem.mCount = 4;
			elem.mUsage = VEU_POSITION;
			vbPos->endAddVertexElements();

			vbPos->initialize(nVerts, 0, &tmpPos[0]);

			// vertex colors
			int nVertColor = fbxMesh->GetElementVertexColorCount();
			for (int i=0; i<nVertColor; ++i) {
				FbxGeometryElementVertexColor* vc = fbxMesh->GetElementVertexColor(i);
				FbxLayerElementArrayTemplate<FbxColor>& colors = vc->GetDirectArray();
				int nColor = colors.GetCount();
				assert(nColor == nVerts);

				if (nColor != nVerts)
					continue;

				VertexBuffer* vbCol = submesh->createVertexBuffer();
				vbCol->beginAddVertexElements();
				VertexElement& elem = vbCol->addVertexElement();
				elem.mType = VET_BYTE;
				elem.mCount = 4;
				elem.mUsage = VEU_COLOR;
				elem.mIndex = i;
				vbCol->endAddVertexElements();

				// FbxColor is in doubles, we need to downconvert to byte
				struct RGBA { unsigned char r, g, b, a; };
				std::vector<RGBA> buf(nVerts);
				for (int i=0; i<nVerts; ++i) {
					buf[i].a = (unsigned char)(colors[i].mAlpha * 255.0);
					buf[i].r = (unsigned char)(colors[i].mRed * 255.0);
					buf[i].g = (unsigned char)(colors[i].mGreen * 255.0);
					buf[i].b = (unsigned char)(colors[i].mBlue* 255.0);
				}

				vbCol->initialize(nVerts, 0, &buf[0]);
			}

			// UVs
			int nUV = fbxMesh->GetElementUVCount();
			for (int i=0; i<nUV; ++i) {
				FbxGeometryElementUV* uv = fbxMesh->GetElementUV(i);
				FbxLayerElementArrayTemplate<FbxVector2>& uvs = uv->GetDirectArray();
				int nCoord = uvs.GetCount();
				assert(nCoord == nVerts);

				if (nCoord != nVerts)
					continue;

				VertexBuffer* vbUV = submesh->createVertexBuffer();
				vbUV->beginAddVertexElements();
				VertexElement& elem = vbUV->addVertexElement();
				elem.mType = VET_FLOAT;
				elem.mCount = 2;
				elem.mUsage = VEU_TEXCOORD;
				elem.mIndex = i;
				vbUV->endAddVertexElements();

				// FbxVector2 is doubles...
				std::vector<Vector2> tmpUV(nVerts);
				for (int i=0; i<nVerts; ++i) {
					tmpUV[i].x = (float)uvs[i][0];
					tmpUV[i].y = (float)uvs[i][1];
				}

				vbUV->initialize(nVerts, 0, &tmpUV[0]);
			}

			// normals
			int nNorm = fbxMesh->GetElementNormalCount();
			for (int i=0; i<nNorm; ++i) {
				FbxGeometryElementNormal* n = fbxMesh->GetElementNormal(i);
				FbxLayerElementArrayTemplate<FbxVector4>& norms = n->GetDirectArray();
				int nNormals = norms.GetCount();
				assert(nNormals == nVerts);

				if (nNormals != nVerts)
					continue;

				VertexBuffer* vbNorm = submesh->createVertexBuffer();
				vbNorm->beginAddVertexElements();
				VertexElement& elem = vbNorm->addVertexElement();
				elem.mType = VET_FLOAT;
				elem.mCount = 4;
				elem.mUsage = VEU_NORMAL;
				elem.mIndex = i;
				vbNorm->endAddVertexElements();

				// FbxVector4 is doubles...
				std::vector<Vector4> tmpNorm(nVerts);
				for (int i=0; i<nVerts; ++i) {
					tmpNorm[i].x = (float)norms[i][0];
					tmpNorm[i].y = (float)norms[i][1];
					tmpNorm[i].z = (float)norms[i][2];
					tmpNorm[i].w = (float)norms[i][3];
				}

				vbNorm->initialize(nVerts, 0, &tmpNorm[0]);
			}

			// tangents
			int nTangent = fbxMesh->GetElementTangentCount();
			for (int i=0; i<nTangent; ++i) {
				FbxGeometryElementTangent* t = fbxMesh->GetElementTangent(i);
				FbxLayerElementArrayTemplate<FbxVector4>& tangents = t->GetDirectArray();
				int nTangents = tangents.GetCount();
				assert(nTangents == nVerts);

				if (nTangents != nVerts)
					continue;

				VertexBuffer* vbTangent = submesh->createVertexBuffer();
				vbTangent->beginAddVertexElements();
				VertexElement& elem = vbTangent->addVertexElement();
				elem.mType = VET_FLOAT;
				elem.mCount = 4;
				elem.mUsage = VEU_TANGENT;
				elem.mIndex = i;
				vbTangent->endAddVertexElements();

				// FbxVector4 is doubles...
				std::vector<Vector4> tmpTang(nVerts);
				for (int i=0; i<nVerts; ++i) {
					tmpTang[i].x = (float)tangents[i][0];
					tmpTang[i].y = (float)tangents[i][1];
					tmpTang[i].z = (float)tangents[i][2];
					tmpTang[i].w = (float)tangents[i][3];
				}

				vbTangent->initialize(nVerts, 0, &tmpTang[0]);
			}

			// binormals
			int nBinormal = fbxMesh->GetElementBinormalCount();
			for (int i=0; i<nBinormal; ++i) {
				FbxGeometryElementBinormal* b = fbxMesh->GetElementBinormal(i);
				FbxLayerElementArrayTemplate<FbxVector4>& binormals = b->GetDirectArray();
				int nBinormals = binormals.GetCount();
				assert(nBinormals == nVerts);

				if (nBinormals != nVerts)
					continue;

				VertexBuffer* vbBinormal = submesh->createVertexBuffer();
				vbBinormal->beginAddVertexElements();
				VertexElement& elem = vbBinormal->addVertexElement();
				elem.mType = VET_FLOAT;
				elem.mCount = 4;
				elem.mUsage = VEU_BINORMAL;
				elem.mIndex = i;
				vbBinormal->endAddVertexElements();

				// FbxVector4 is doubles...
				std::vector<Vector4> tmpBinorm(nVerts);
				for (int i=0; i<nVerts; ++i) {
					tmpBinorm[i].x = (float)binormals[i][0];
					tmpBinorm[i].y = (float)binormals[i][1];
					tmpBinorm[i].z = (float)binormals[i][2];
					tmpBinorm[i].w = (float)binormals[i][3];
				}

				vbBinormal->initialize(nVerts, 0, &tmpBinorm[0]);
			}

			// finally the material(s)
			int nMaterial = node->GetMaterialCount();

			// we do not yet support import of meshes with multiple materials
			assert(nMaterial == 1);
			if (nMaterial != 1)
				// TODO: alert? not importing a material is a warning, not an error, so return true
				return true;

			FbxSurfaceMaterial* mtl = node->GetMaterial(0);
			const char* mtlName = mtl->GetName();

			if (table.find(String(mtlName)) == table.end()) {
				Gfx::Material* gfxMtl = importMaterial(mtl, imp);
				if (gfxMtl) {
					imp.addDep(gfxMtl, mtlName);
					asset->setMaterial(gfxMtl);
					table[mtlName] = gfxMtl;
				}
			}

			return true;
		}

		static void importNode(FbxNode* node, StaticMeshAsset* asset, ImportedAsset& imp, MaterialTable& table, const char* filepath) 
		{
			using namespace Gfx;

			FbxNodeAttribute* nodeAttr = node->GetNodeAttribute();
			if (!nodeAttr)
				return;

			FbxNodeAttribute::EType nodeType = nodeAttr->GetAttributeType();
			switch (nodeType) {
				case FbxNodeAttribute::eSkeleton:
					break;
				case FbxNodeAttribute::eMesh:
					if (!importMesh(node, asset, imp, table, filepath))
						return;
					break;
			}

			int nChild = node->GetChildCount();

			for(int i = 0; i < nChild; i++) {
				importNode(node->GetChild(i), asset, imp, table, filepath);
			}
		}

		StaticMeshAsset* importStaticMesh(ImportedAsset& imp, const char* filepath)
		{
			StaticMeshAsset* asset = 0;

			FbxManager* fbx = FbxManager::Create();
			FbxIOSettings* ios = FbxIOSettings::Create(fbx, IOSROOT);
			fbx->SetIOSettings(ios);
			FbxImporter* importer = FbxImporter::Create(fbx, "");

			if (importer->Initialize(filepath, -1, ios)) {
				FbxScene* scene = FbxScene::Create(fbx, "scene");
				importer->Import(scene);
				importer->Destroy();

				FbxNode* node = scene->GetRootNode();
				MaterialTable table;

				asset = TD_NEW StaticMeshAsset;
				asset->initialize();

				for (int i=0; i<node->GetChildCount(); ++i) {
					importNode(node->GetChild(i), asset, imp, table, filepath);
				}
			}

			fbx->Destroy();

			return asset;
		}

	} // namespace Tools
} // namespace Teardrop
