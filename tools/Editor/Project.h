/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PROJECT_H)
#define PROJECT_H

#include "Util/_String.h"
#include "FastDelegate.h"
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

			fastdelegate::FastDelegate1<const char*> NameChanged;
			fastdelegate::FastDelegate1<PackageManager*> PackageLoaded;

		protected:
			String mName;
			String mPath;
			int mVersion;

			PackageManagers mPackageManagers;
		};
	}
}

#endif // PROJECT_H
