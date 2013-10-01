/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "AssetImport.h"
#include "Metadata.h"
#include "Asset/StaticMeshAsset.h"
#include "fbxsdk.h"

using namespace fbxsdk_2014_1;

namespace Teardrop {
	namespace Tools {

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

				FbxNode* root = scene->GetRootNode();
				if(root) {
					for(int i = 0; i < root->GetChildCount(); i++) {
						FbxNode* node = root->GetChild(i);
						const char* name = node->GetName();

						int nMtl = node->GetMaterialCount();
						for (int m=0; m<nMtl; ++m) {
							FbxSurfaceMaterial* mtl = node->GetMaterial(m);
							const char* mtlName = mtl->GetName();
						}
					}
				}
			}

			fbx->Destroy();

			return asset;
		}

	} // namespace Tools
} // namespace Teardrop
