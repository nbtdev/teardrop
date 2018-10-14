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
			// retrieve package's Executable instance (if any); returns NULL if none
			Executable* executable();

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
