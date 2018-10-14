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
		typedef std::list<Reflection::Object*> InternalDependencies;

		class ImportedAsset 
		{
		public:
			ImportedAsset();
			~ImportedAsset();
			
			bool isValid();
			void setAsset(Asset* asset, Metadata* metadata);
			void addDep(Reflection::Object* obj, const String& name);
			void addInternalDep(Reflection::Object* obj);
			void addDep(Asset* asset, const String& name, const String& filepath);
			int numDependencies();
			int numInternalDependencies();
			Dependencies& dependencies();
			InternalDependencies& internalDependencies();
			Asset* asset();
			Metadata* metadata();

		protected:
			// all imported assets will have these set
			Asset* mAsset;
			Metadata* mMetadata;

			// assets that pull in other assets will have these set
			Dependencies mDeps;

			// assets that create internal dependencies will have these set
			InternalDependencies mInternalDeps;

		private:
			ImportedAsset(const ImportedAsset& other);
			ImportedAsset& operator=(const ImportedAsset& other);
		};
	}
}

#endif // IMPORTEDASSET_INCLUDED
