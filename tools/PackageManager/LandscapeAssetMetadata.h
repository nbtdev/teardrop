/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(LANDSCAPEASSETMETADATA_INCLUDED)
#define LANDSCAPEASSETMETADATA_INCLUDED

#include "PackageManager/AssetMetadata.h"
#include <map>

namespace Teardrop 
{
	class LandscapeAsset;

	namespace Tools
	{
		class LandscapeAssetMetadata : public AssetMetadata
		{
		public:
			TD_CLASS(LandscapeAssetMetadata, AssetMetadata);

			LandscapeAssetMetadata();
			LandscapeAssetMetadata(LandscapeAsset* asset);
			~LandscapeAssetMetadata();

			void generateThumbnail();

			TD_DECLARE_ALLOCATOR();

		protected:
		};
	}
}

#endif // ASSETMETADATA_INCLUDED
