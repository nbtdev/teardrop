/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PREFERENCES_INCLUDED)
#define PREFERENCES_INCLUDED

#include "Util/_String.h"
#include <list>

namespace Teardrop
{
	namespace Tools
	{
		class Preferences
		{
		public:
			Preferences();
			~Preferences();

			struct General
			{
				General();
				~General();

				bool mLoadLastProject;
			};

			bool load(const char* prefsFilename=0);
			bool save(const char* prefsFilename=0);

			General& general();

			typedef std::list<String> ProjectList;
			void addProject(const String& projectPathname);
			const ProjectList& projectList();

		private:
			General mGeneral;

			ProjectList mLastProjects;
		};
	}
}

#endif // PREFERENCES_INCLUDED
