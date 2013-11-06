/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PACKAGEMANAGER_INCLUDED)
#define PACKAGEMANAGER_INCLUDED

#include "Package/DeferredResolution.h"
#include "PackageManager/ImportedAsset.h"
#include <utility>

namespace Teardrop 
{
	class Package;
	class Executable;
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

		class PackageManager
		{
		public:
			PackageManager();
			~PackageManager();

			// create new assets/objects from filesystem data
			void importAsset(/*out*/ImportedAsset& imp, Folder* folder, const char* filepath, const Reflection::ClassDef* assetClass);
			// create/return new object instance from ClassDef
			std::pair<Reflection::Object*, Metadata*> createObject(Folder* folder, const Reflection::ClassDef* classDef);
			// make package executable using ClassDef
			Executable* makeExecutable(const Reflection::ClassDef* classDef);

			void initializeAllObjects();

			// delete a folder, optionally recursively; non-recursive deletes simply transfer objects and folders to the
			// deleted folder's parent
			void remove(Folder* folder, bool bRecursive=false);
			// delete an object from the package 
			void remove(Reflection::Object* obj);

			// load/save the package
			bool save(const String& path);
			bool load(const String& path, DeferredObjectResolves& deferred, ObjectIdToObject& lut);
			String packageFilename();

			PackageMetadata* metadata();
			void getAllMetadata(std::list<Metadata*>& metaList) const;

			void findAllOf(std::list<Reflection::Object*>& objs, Reflection::ClassDef* isA, bool includeDerived);

		protected:
			Package* mPackage;
			PackageMetadata* mMetadata;
		};
	}
}

#endif // PACKAGEMANAGER_INCLUDED
