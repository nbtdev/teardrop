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

#if !defined(SYSTEMMANAGER_H_INCLUDED)
#define SYSTEMMANAGER_H_INCLUDED

#include "Util/System.h"
#include <set>

namespace Teardrop
{
	class SystemManager
	{
		typedef std::set<System*> RegisteredSystems;

		struct Registration
		{
			System* pActive;
			RegisteredSystems registeredSystems;
		};

		Registration m_systems[System::SYSTEM_TYPE_MAX];

	public:
		SystemManager();
		~SystemManager();

		// register a system implementation
		void registerSystem(System* pSystem);
		// activate a system implementation fro the given type
		void activateSystem(System::Type type, System* pSystem);
		// convenience method to do both in one step (activates for all of the System's supported types)
		void registerAndActivateSystem(System* pSystem);
		// unregister a system implementation for the specified type
		void unregisterSystem(System::Type type, System* pSystem);
		// get active system implementation for requested type
		System* getActiveSystem(System::Type type);
	};
}

#endif // SYSTEMMANAGER_H_INCLUDED
