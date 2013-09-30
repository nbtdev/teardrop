/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(HEIGHTFIELDASSETMETADATA_INCLUDED)
#define HEIGHTFIELDASSETMETADATA_INCLUDED

#include "PackageManager/AssetMetadata.h"
#include <map>

namespace Teardrop 
{
	class HeightfieldAsset;

	namespace Tools
	{
		class HeightfieldAssetMetadata : public AssetMetadata
		{
		public:
			TD_CLASS(HeightfieldAssetMetadata, AssetMetadata);

			HeightfieldAssetMetadata();
			HeightfieldAssetMetadata(HeightfieldAsset* asset);
			~HeightfieldAssetMetadata();

			void generateThumbnail();

			TD_DECLARE_ALLOCATOR();

		protected:
			void onPropertyChanged(const Reflection::PropertyDef* prop);
		};
	}
}

#endif // ASSETMETADATA_INCLUDED
