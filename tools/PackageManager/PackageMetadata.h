/******************************************************************************
Copyright (c) 2018 Teardrop Games

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
******************************************************************************/

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
			int serialize(Package* pkg, Stream& strm);
			int deserialize(Package* pkg, Stream& strm);

			Folder* rootFolder();

			Metadata* add(/*out*/String& uuid, /*in*/Folder* parent, /*in*/Asset* asset, /*in*/const char* assetSourcePath);
			Metadata* add(/*out*/String& uuid, /*in*/Folder* parent, /*in*/Reflection::Object* object);
			void remove(Reflection::Object* object);
			void remove(const String& objectId);
			void move(Reflection::Object* object, Folder* oldParent, Folder* newParent);
			void move(Folder* folder, Folder* newParent);
			Folder* newFolder(const String& name, Folder* parent=0);
			Folder* deleteFolder(Folder* folder, bool bRecursive=false);
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
