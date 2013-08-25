/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PACKAGEMETADATA_INCLUDED)
#define PACKAGEMETADATA_INCLUDED

#include "Memory/Memory.h"
#include "Util/_String.h"
#include <map>

namespace Teardrop 
{
	class Package;
	class Asset;

	namespace Reflection
	{
		class Object;
	}

	namespace Tools
	{
		class Folder;

		class PackageMetadata
		{
		public:
			PackageMetadata(Package* package);
			~PackageMetadata();

			Folder* rootFolder();
			const String& packageName();
			void renamePackage(const String& name);

			void add(/*out*/String& uuid, /*in*/Folder* parent, /*in*/Asset* asset, /*in*/const char* assetSourcePath);
			void add(/*out*/String& uuid, /*in*/Folder* parent, /*in*/Reflection::Object* object);
			void remove(Reflection::Object* object);
			void remove(const String& objectId);
			void move(Reflection::Object* object, Folder* oldParent, Folder* newParent);
			Folder* newFolder(const String& name, Folder* parent=0);
			Folder* deleteFolder(Folder* folder);
			void renameFolder(Folder* folder, const String& name);

			const String& findAssetSourcePath(Asset* asset);
			const String& findAssetSourcePath(const String& assetId);
			const String& findId(Reflection::Object* object);

		protected:
			Folder* mRoot;
			Package* mPackage;

			// the package itself doesn't need a name, but we do, so store it here
			String mPackageName;

			// maps object ID to object instance
			typedef std::map<String /*id*/, Reflection::Object* /*instance*/> ObjectIdToObjectMap;
			ObjectIdToObjectMap mObjectIdToObjectMap;

			// maps object instance to object ID
			typedef std::map<Reflection::Object* /*instance*/, String /*id*/> ObjectToObjectIdMap;
			ObjectToObjectIdMap mObjectToObjectIdMap;

			// maps asset ID to source path
			typedef std::map<String /*id*/, String /*path*/> AssetIdToPathMap;
			AssetIdToPathMap mAssetIdToPathMap;

			// maps asset instance (pointer) to source path
			typedef std::map<Asset* /*instance*/, String /*path*/> AssetToPathMap;
			AssetToPathMap mAssetToPathMap;

			// maps object instance to Folder instance
			typedef std::map<Reflection::Object* /*instance*/, Folder* /*folder*/> ObjectToFolderMap;
			ObjectToFolderMap mObjectToFolderMap;
		};
	}
}

#endif // PACKAGEMETADATA_INCLUDED
