/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(ATTRIBUTEMAPASSETMETADATA_INCLUDED)
#define ATTRIBUTEMAPASSETMETADATA_INCLUDED

#include "PackageManager/AssetMetadata.h"
#include <map>

namespace Teardrop 
{
	class AttributeMapAsset;

	namespace Tools
	{
		class AttributeMapAssetMetadata : public AssetMetadata
		{
		public:
			TD_CLASS(AttributeMapAssetMetadata, AssetMetadata);

			AttributeMapAssetMetadata();
			AttributeMapAssetMetadata(AttributeMapAsset* asset);
			~AttributeMapAssetMetadata();

			void generateThumbnail();

			TD_DECLARE_ALLOCATOR();

		protected:
			AttributeMapAsset* mAMAsset;

			void onPropertyChanged(const Reflection::PropertyDef* prop);
		};
	}
}

#endif // ATTRIBUTEMAPASSETMETADATA_INCLUDED
