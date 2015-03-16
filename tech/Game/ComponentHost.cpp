/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "ComponentHost.h"
#include "Component.h"
#include "Reflection/Reflection.h"
#include "Util/Environment.h"
#include <algorithm>

using namespace Teardrop;
using namespace Reflection;
//---------------------------------------------------------------------------
ComponentHost::ComponentHost()
{
}
//---------------------------------------------------------------------------
ComponentHost::~ComponentHost()
{
}
//---------------------------------------------------------------------------
bool ComponentHost::addComponent(Component* pComp)
{
	if (!pComp)
		return false;

	if (!pComp->getServerComponent() && Environment::get().isServer)
		return false;

	pComp->setHost(this);

	m_components.insert(Components::value_type(
		pComp->getDerivedClassDef(), pComp));

	return true;
}
//---------------------------------------------------------------------------
int ComponentHost::getComponents(Component** comps, int nComps) const
{
	if (!comps || !nComps) 
		return int(m_components.size());

	int compsReturned = std::min(int(m_components.size()), nComps);
	Components::const_iterator it = m_components.begin();

	for (int i=0; i<compsReturned; ++i) {
		comps[i] = it->second;
		++it;
	}

	return compsReturned;
}
//---------------------------------------------------------------------------
void ComponentHost::removeComponent(Component* pComp)
{
	Components::iterator it = m_components.find(pComp->getDerivedClassDef());

	while (it != m_components.end())
		if (it->second == pComp)
			m_components.erase(it);
}
//---------------------------------------------------------------------------
size_t ComponentHost::findComponents(
	/*in*/	const ClassDef* pClass,
	/*out*/	Component** pComponents,
	/*in*/	size_t numComponents,
	/*in*/	bool bSearchInherited
) const
{
	size_t found = 0;
	if (bSearchInherited)
	{
		// search through all of our components
		for (Components::const_iterator it = m_components.begin(); 
			it != m_components.end(); ++it)
		{
			if (it->second->getDerivedClassDef()->isA(pClass))
			{
				pComponents[found++] = it->second;
			}
		}
	}
	else
	{
		Components::const_iterator it = m_components.lower_bound(const_cast<ClassDef*>(pClass));
		Components::const_iterator end = m_components.upper_bound(const_cast<ClassDef*>(pClass));
		while (it != end && found < numComponents)
		{
			// do not return components that should not be running on a server
			if (Environment::get().isServer && 
				it->second->getServerComponent() == false)
				continue;

			// otherwise, since we know we found components with this class, 
			// append them to the component array
			pComponents[found++] = it->second;

			++it;
		}
	}

	return found;
}
//---------------------------------------------------------------------------
void ComponentHost::findComponents(
	/*in*/	const ClassDef* pClass,
	/*out*/	ComponentList& components,
	/*in*/	bool bSearchInherited
) const
{
	// do not clear the incoming list; leave that to the caller

	if (bSearchInherited)
	{
		// search through all of our components
		for (Components::const_iterator it = m_components.begin(); 
			it != m_components.end(); ++it)
		{
			if (it->second->getDerivedClassDef()->isA(pClass))
			{
				components.push_back(it->second);
			}
		}
	}
	else
	{
		Components::const_iterator it = m_components.lower_bound(const_cast<ClassDef*>(pClass));
		Components::const_iterator end = m_components.upper_bound(const_cast<ClassDef*>(pClass));
		while (it != end)
		{
			// do not return components that should not be running on a server
			if (Environment::get().isServer && 
				it->second->getServerComponent() == false)
				continue;

			// otherwise, since we know we found components with this class, 
			// append them to the component list
			components.push_back(it->second);

			++it;
		}
	}
}
