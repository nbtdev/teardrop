/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(IMPORTEDASSET_INCLUDED)
#define IMPORTEDASSET_INCLUDED

#include "Util/_String.h"
#include <list>

namespace Teardrop 
{
	class Asset;
	class GfxMaterial;

	namespace Reflection
	{
		class Object;
	}

	namespace Tools
	{
		class Metadata;

		struct Dependency
		{
			Reflection::Object* mObject;
			Metadata* mMetadata;
			String mName;
			String mSourcePath; // if applicable

			Dependency() : mObject(0), mMetadata(0) {}
			Dependency(const Dependency& other) { *this = other; }
			Dependency& operator=(const Dependency& other) {
				mObject = other.mObject;
				mMetadata = other.mMetadata;
				mName = other.mName;
				mSourcePath = other.mSourcePath;
				return *this;
			}
		};
		typedef std::list<Dependency> Dependencies;

		class ImportedAsset 
		{
		public:
			ImportedAsset();
			~ImportedAsset();
			
			bool isValid();
			void setAsset(Asset* asset, Metadata* metadata);
			void addDep(Reflection::Object* obj, const String& name);
			void addDep(Asset* asset, const String& name, const String& filepath);
			int numDependencies();
			Dependencies& dependencies();
			Asset* asset();
			Metadata* metadata();

		protected:
			// all imported assets will have these set
			Asset* mAsset;
			Metadata* mMetadata;

			// assets that pull in other assets will have these set
			Dependencies mDeps;

		private:
			ImportedAsset(const ImportedAsset& other);
			ImportedAsset& operator=(const ImportedAsset& other);
		};
	}
}

#endif // IMPORTEDASSET_INCLUDED
