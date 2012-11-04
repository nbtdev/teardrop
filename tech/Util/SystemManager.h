/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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