/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(ASSETIMPORT_INCLUDED)
#define ASSETIMPORT_INCLUDED

namespace Teardrop 
{
	class TextureAsset;
	class LandscapeAsset;
	class StaticMeshAsset;

	namespace Tools
	{
		class ImportedAsset;

		enum TextureAssetType {
			TEXTUREASSET_TYPE_BCX,			// compress to BCx (default in editor)
			TEXTUREASSET_TYPE_NATIVE,		// compress to native type for target 
			TEXTUREASSET_TYPE_UNCOMPRESSED,	// no compression (heightfields, etc)

			TEXTUREASSET_TYPE_MAX,
		};

		enum LandscapeAssetType {
			LANDSCAPEASSET_TYPE_L3DT,		// Landscape created in L3DT

			LANDSCAPEASSET_TYPE_MAX,
		};

		TextureAsset* importTexture(const char* filepath, TextureAssetType type);
		LandscapeAsset* importLandscape(/*out*/ImportedAsset& imp, /*in*/const char* filepath, LandscapeAssetType type);
		StaticMeshAsset* importStaticMesh(/*out*/ImportedAsset& imp, /*in*/const char* filepath);
	}
}

#endif // ASSETIMPORT_INCLUDED
