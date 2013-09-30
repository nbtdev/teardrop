/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TEXTUREASSETMETADATA_INCLUDED)
#define TEXTUREASSETMETADATA_INCLUDED

#include "PackageManager/AssetMetadata.h"
#include <map>

namespace Teardrop 
{
	class TextureAsset;

	namespace Tools
	{
		class TextureAssetMetadata : public AssetMetadata
		{
		public:
			TD_CLASS(TextureAssetMetadata, AssetMetadata);
			TD_PROPERTY(GenerateMipmaps, "Generate mipmaps for texture asset", bool, false, 0);

			TextureAssetMetadata();
			TextureAssetMetadata(TextureAsset* asset);
			~TextureAssetMetadata();

			void generateThumbnail();

			TD_DECLARE_ALLOCATOR();

		protected:
			void onPropertyChanged(const Reflection::PropertyDef* prop);
		};
	}
}

#endif // ASSETMETADATA_INCLUDED
