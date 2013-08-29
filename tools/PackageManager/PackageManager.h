/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PACKAGEMANAGER_INCLUDED)
#define PACKAGEMANAGER_INCLUDED

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
			Asset* importAsset(Folder* folder, const char* filepath, const Reflection::ClassDef* assetClass);

			// add/remove existing assets/objects
			// add object that comes from another package
			void add(Reflection::Object* object, PackageManager* origin, Folder* newParent);
			// add unassociated object
			void add(Reflection::Object* object, Folder* parent);
			// create/add new folder
			void add(const String& name, Folder* parent);
			// remove object from our package
			void remove(Reflection::Object* object);

			// load/save the package
			bool save(const String& path);
			bool load(const String& path);
			String packageFilename();

			PackageMetadata* metadata();

		protected:
			Package* mPackage;
			PackageMetadata* mMetadata;
		};
	}
}

#endif // PACKAGEMANAGER_INCLUDED
