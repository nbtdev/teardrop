/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PACKAGEMETADATA_INCLUDED)
#define PACKAGEMETADATA_INCLUDED

#include "PackageManager/Metadata.h"
#include "Package/PackageMetadataSerializer.h"
#include <map>

class TiXmlElement;

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
		class Metadata;

		class PackageMetadata : public Metadata, public PackageMetadataSerializer
		{
		public:
			TD_CLASS(PackageMetadata, Metadata);

			PackageMetadata();
			PackageMetadata(Package* package);
			~PackageMetadata();

			// PackageMetadataSerializer implementation
			void serialize(Package* pkg, Stream& strm);
			void deserialize(Package* pkg, Stream& strm);

			Folder* rootFolder();

			Metadata* add(/*out*/String& uuid, /*in*/Folder* parent, /*in*/Asset* asset, /*in*/const char* assetSourcePath);
			Metadata* add(/*out*/String& uuid, /*in*/Folder* parent, /*in*/Reflection::Object* object);
			void remove(Reflection::Object* object);
			void remove(const String& objectId);
			void move(Reflection::Object* object, Folder* oldParent, Folder* newParent);
			void move(Folder* folder, Folder* newParent);
			Folder* newFolder(const String& name, Folder* parent=0);
			Folder* deleteFolder(Folder* folder);
			void renameFolder(Folder* folder, const String& name);

			void getAllMetadata(std::list<Metadata*>& metaList) const;
			Metadata* findObjectMetadata(Reflection::Object* object);
			Reflection::Object* findObject(const String& id);

			TD_DECLARE_ALLOCATOR();

		protected:
			Folder* mRoot;
			Package* mPackage;

			// maps object ID to object instance
			typedef std::map<String /*id*/, Reflection::Object* /*instance*/> ObjectIdToObjectMap;
			ObjectIdToObjectMap mObjectIdToObjectMap;

			// maps object instance to object metadata
			typedef std::map<Reflection::Object* /*instance*/, Metadata* /*metadata*/> ObjectToMetadataMap;
			ObjectToMetadataMap mObjectToMetadataMap;

			// maps object instance to Folder instance
			typedef std::map<Reflection::Object* /*instance*/, Folder* /*folder*/> ObjectToFolderMap;
			ObjectToFolderMap mObjectToFolderMap;

			void onPropertyChanged(const Reflection::PropertyDef* prop);

			// this is here for the metadata deserializer
			void loadFolders(TiXmlElement* elem, Folder* parent, Package* pkg);
		};
	}
}

#endif // PACKAGEMETADATA_INCLUDED
