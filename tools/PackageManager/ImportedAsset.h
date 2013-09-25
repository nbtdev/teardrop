/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(IMPORTEDASSET_INCLUDED)
#define IMPORTEDASSET_INCLUDED

#include "Util/_String.h"

namespace Teardrop 
{
	class Asset;

	namespace Tools
	{
		class Metadata;

		struct ImportedAsset 
		{
			ImportedAsset();
			~ImportedAsset();
			ImportedAsset(const ImportedAsset& other);
			ImportedAsset& operator=(const ImportedAsset& other);
			
			bool isValid();
			void addDep(Asset* asset, const String& filepath, const String& name);

			// all assets will have these set
			Asset* mAsset;
			Metadata* mMetadata;

			// assets that pull in other assets will have these set
			Asset** mDeps;
			Metadata** mDepsMetadata;
			String* mDepsFilepath;
			String* mDepsMetaName;
			int mNumDeps;
		};
	}
}

#endif // IMPORTEDASSET_INCLUDED
