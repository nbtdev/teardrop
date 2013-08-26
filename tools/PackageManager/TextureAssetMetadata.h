/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TEXTUREASSETMETADATA_INCLUDED)
#define TEXTUREASSETMETADATA_INCLUDED

#include "PackageManager/Metadata.h"
#include <map>

namespace Teardrop 
{
	class TextureAsset;

	namespace Tools
	{
		class TextureAssetMetadata : public Metadata
		{
		public:
			TD_CLASS(TextureAssetMetadata, Metadata);
			TD_PROPERTY(SourcePath, "Filesystem path to texture asset source data", String, "(undefined)", "FileChooser|ReadOnly");
			TD_PROPERTY(GenerateMipmaps, "Generate mipmaps for texture asset", bool, false, 0);

			TextureAssetMetadata();
			TextureAssetMetadata(TextureAsset* asset);
			~TextureAssetMetadata();

			TD_DECLARE_ALLOCATOR();

		protected:
			TextureAsset* mTexAsset;

			void onPropertyChanged(const Reflection::PropertyDef* prop);
		};
	}
}

#endif // ASSETMETADATA_INCLUDED
