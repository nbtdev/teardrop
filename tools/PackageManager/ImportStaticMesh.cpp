/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "AssetImport.h"
#include "Metadata.h"
#include "ImportedAsset.h"
#include "Asset/StaticMeshAsset.h"
#include "Asset/TextureAsset.h"
#include "Gfx/Material.h"
#include "Gfx/MaterialOutput.h"
#include "Gfx/Sampler2DExpression.h"
#include "Gfx/Connection.h"
#include "Gfx/Attribute.h"
#include "Util/UUID.h"
#include "Util/_String.h"
#include "fbxsdk.h"
#include <map>

using namespace fbxsdk_2014_1;

namespace Teardrop {
	namespace Tools {

		typedef std::map<String, Gfx::Material*> MaterialTable;

		static void importNode(FbxNode* node, StaticMeshAsset* asset, ImportedAsset& imp, MaterialTable& table) 
		{
			using namespace Gfx;

			int nChild = node->GetChildCount();

			for(int i = 0; i < nChild; i++) {
				importNode(node->GetChild(i), asset, imp, table);
			}

			const char* name = node->GetName();
			int nMtl = node->GetMaterialCount();
			for (int m=0; m<nMtl; ++m) {
				FbxSurfaceMaterial* mtl = node->GetMaterial(m);
				const char* mtlName = mtl->GetName();

				if (table.find(String(mtlName)) != table.end()) {
					// then we already have it, continue
					continue;
				}

				MaterialOutput* output = TD_NEW MaterialOutput;
				UUID uuid;
				output->setObjectId(uuid);
				output->initialize();
				imp.addInternalDep(output);

				Material* gfxMtl = TD_NEW Material;
				gfxMtl->setOutput(output);
				table[mtlName] = gfxMtl;

				// TODO: handle a static mesh with multiple materials (needs to be broken up into 
				// multiple static (sub)meshes, for a 1:1 relationship with materials)
				asset->setMaterial(gfxMtl);

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
										conn->setFromExpression(expr); conn->setFromAttribute("RGBA");
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

				imp.addDep(gfxMtl, mtlName);
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

				importNode(node, asset, imp, table);
			}

			fbx->Destroy();

			return asset;
		}

	} // namespace Tools
} // namespace Teardrop
