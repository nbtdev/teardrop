/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

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
-----------------------------------------------------------------------------
*/

#include "ComponentHost.h"
#include "Component.h"
#include "Reflection/include/Reflection.h"
#include "Util/include/Environment.h"

using namespace CoS;
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
