/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(ASSETMETADATA_INCLUDED)
#define ASSETMETADATA_INCLUDED

#include "PackageManager/Metadata.h"
#include <map>

namespace Teardrop 
{
	namespace Tools
	{
		class AssetMetadata : public Metadata
		{
		public:
			TD_CLASS(AssetMetadata, Metadata);
			TD_PROPERTY(SourcePath, "Filesystem path to asset source data", String, "(undefined)", "FileChooser|ReadOnly");

			AssetMetadata();
			~AssetMetadata();

			TD_DECLARE_ALLOCATOR();

		protected:
		};
	}
}

#endif // ASSETMETADATA_INCLUDED
