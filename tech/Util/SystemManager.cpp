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
		m_systems[types[i]].registeredSystems.insert(RegisteredSystems::value_type(pSystem));
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
