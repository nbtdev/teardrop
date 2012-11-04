/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "SystemManager.h"

using namespace Teardrop;

SystemManager::SystemManager()
{
	for (int i=0; i<System::SYSTEM_TYPE_MAX; ++i)
		m_systems[i].pActive = 0;
}

SystemManager::~SystemManager()
{
	// we didn't allocate anything so nothing to clean up
}

void SystemManager::registerSystem(System* pSystem)
{
	if (!pSystem)
		return; // TODO: throw?

	System::Type types[16];
	int typeCount = 16;

	pSystem->getTypes(types, typeCount);

	for (int i=0; i<typeCount; ++i)
	{
		m_systems[i].registeredSystems.insert(RegisteredSystems::value_type(pSystem));
	}
}

void SystemManager::activateSystem(System::Type type, System* pSystem)
{
	// bounds check
	if (type < 0 || type >= System::SYSTEM_TYPE_MAX)
		return; // TODO: throw?

	// ensure the system is registered
	RegisteredSystems::iterator it = m_systems[type].registeredSystems.find(pSystem);
	if (it == m_systems[type].registeredSystems.end())
		return; // TODO: throw?

	// finally, activate the system
	m_systems[type].pActive = pSystem;
}

void SystemManager::registerAndActivateSystem(System* pSystem)
{
	// same as register method, except we also activate as we go

	if (!pSystem)
		return; // TODO: throw?

	System::Type types[16];
	int typeCount = 16;

	pSystem->getTypes(types, typeCount);

	for (int i=0; i<typeCount; ++i)
	{
		m_systems[i].registeredSystems.insert(RegisteredSystems::value_type(pSystem));
		m_systems[i].pActive = pSystem;
	}
}

void SystemManager::unregisterSystem(System::Type /*type*/, System* /*pSystem*/)
{

}

System* SystemManager::getActiveSystem(System::Type type)
{
	// bounds check
	if (type < 0 || type >= System::SYSTEM_TYPE_MAX)
		return 0; // TODO: throw?

	return m_systems[type].pActive;
}
