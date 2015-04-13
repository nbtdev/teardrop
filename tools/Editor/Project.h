/******************************************************************************
Copyright (c) 2015 Teardrop Games

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

#if !defined(PROJECT_H)
#define PROJECT_H

#include "Util/_String.h"
#include "Util/Event.h"
#include <list>

namespace Teardrop
{
	namespace Tools
	{
		class PackageManager;
		class ProgressFeedback;

		class Project
		{
		public:
			Project();
			~Project();
			
			static const int CURRENT_VERSION;

			const String& name();
			const String& path();
			void setPath(const String& path);
			void rename(const String& newName);
			bool write(ProgressFeedback* feedback=0);
			bool read(ProgressFeedback* feedback=0);
			int version();
			// create new, empty package
			PackageManager* createPackage();
			// load existing package from disk, copying into project's packages/ directory if necessary (and possible)
			PackageManager* addPackage(const char* packagePath);
			// remove package from project (but not delete from disk)
			void removePackage(PackageManager* pkgMgr);

			typedef std::list<PackageManager*> PackageManagers;
			const PackageManagers& packages();

            Event<const char*> NameChanged;
            Event<PackageManager*> PackageLoaded;

		protected:
			String mName;
			String mPath;
			int mVersion;

			PackageManagers mPackageManagers;
		};
	}
}

#endif // PROJECT_H
