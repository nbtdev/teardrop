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

class TiXmlElement;

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
				void load(TiXmlElement&);
				void save(TiXmlElement&);

				bool mLoadLastProject;
			};

			struct Rendering
			{
				Rendering();
				~Rendering();
				void load(TiXmlElement&);
				void save(TiXmlElement&);

				enum Engine {
					ENGINE_OPENGL=0,
					ENGINE_D3D9,
					ENGINE_D3D11,
				};

				Engine mEngine;
			};

			bool load(const char* prefsFilename=0);
			bool save(const char* prefsFilename=0);

			General& general();
			Rendering& rendering();

			typedef std::list<String> ProjectList;
			void addProject(const String& projectPathname);
			const ProjectList& projectList();

		private:
			General mGeneral;
			Rendering mRendering;

			ProjectList mLastProjects;
		};
	}
}

#endif // PREFERENCES_INCLUDED
