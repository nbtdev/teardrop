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

#if !defined(PREFERENCES_INCLUDED)
#define PREFERENCES_INCLUDED

#include "Util/_String.h"
#include "Util/UUID.h"
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

				UUID mEngineId;
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
