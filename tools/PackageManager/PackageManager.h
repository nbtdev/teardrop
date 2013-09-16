/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PACKAGEMANAGER_INCLUDED)
#define PACKAGEMANAGER_INCLUDED

#include "Package/DeferredResolution.h"
#include <utility>

namespace Teardrop 
{
	class Package;
	class Asset;
	class TextureAsset;
	class String;

	namespace Reflection
	{
		class ClassDef;
		class Object;
	}

	namespace Tools
	{
		class PackageMetadata;
		class Metadata;
		class Thumbnail;
		class Folder;

		enum TextureAssetType {
			TEXTUREASSET_TYPE_BCX,		// compress to BCx (default in editor)
			TEXTUREASSET_TYPE_NATIVE,	// compress to native type for target 

			TEXTUREASSET_TYPE_MAX,
		};

		TextureAsset* importTexture(const char* filepath, TextureAssetType type);

		class PackageManager
		{
		public:
			PackageManager();
			~PackageManager();

			// create new assets/objects from filesystem data
			std::pair<Asset*, Metadata*> importAsset(Folder* folder, const char* filepath, const Reflection::ClassDef* assetClass);
			// create/return new object instance from ClassDef
			std::pair<Reflection::Object*, Metadata*> createObject(Folder* folder, const Reflection::ClassDef* classDef);

			// load/save the package
			bool save(const String& path);
			bool load(const String& path, DeferredObjectResolves& deferred, ObjectIdToObject& lut);
			String packageFilename();

			PackageMetadata* metadata();

		protected:
			Package* mPackage;
			PackageMetadata* mMetadata;
		};
	}
}

#endif // PACKAGEMANAGER_INCLUDED
