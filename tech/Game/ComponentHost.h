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

#if !defined(COMPONENTHOST_INCLUDED)
#define COMPONENTHOST_INCLUDED

#include <map>
#include <list>

namespace Teardrop
{
	class Transform;
	class AABB;
	class Component;

	namespace Reflection
	{
		class ClassDef;
	}

	class ComponentHost
	{
	public:
		ComponentHost();
		virtual ~ComponentHost();

		typedef std::multimap<const Reflection::ClassDef*, Component*> Components;
		typedef std::list<Component*> ComponentList;
		// add a component to this host
		bool addComponent(Component* pComp);
		// remove a component from this host
		void removeComponent(Component* pComp); // THIS CAN BE EXPENSIVE
		// get all components contained within
		const Components& getComponents() const { return m_components; }
		// get all components contained within; returns number of components found. To 
		// get component count, pass zero/null for comps and/or nComps
		int getComponents(/*out*/ Component** comps, /*in*/ int nComps) const;
		// find a particular component by class; returns number of components found,
		// and the Component array in ppComponents, max length of numComponents
		virtual size_t findComponents(
			/*in*/		const Reflection::ClassDef* pClass,
			/*out*/		Component** pComponents,
			/*in*/		size_t numComponents = 1,
			/*in*/		bool bSearchInherited = false // THIS CAN BE EXPENSIVE
			) const;
		// find a particular component by class; fills in Components argument with 
		// all matching components
		virtual void findComponents(
			/*in*/		const Reflection::ClassDef* pClass,
			/*out*/		ComponentList& components,
			/*in*/		bool bSearchInherited = false // THIS CAN BE EXPENSIVE
			) const;

		//! return WORLD SPACE transform of the host
		virtual const Transform& getTransformWS() = 0;
		virtual void setBoundingBox(const AABB& aabb) = 0;
		virtual bool isOfType(Reflection::ClassDef*) = 0;

		////! called on host when addComponent() is called
		//virtual void notifyComponentAdded(Component*);
		////! called on host when removeComponent() is called
		//virtual void notifyComponentRemoved(Component*);

	protected:
		Components m_components;
	};
}

#endif // COMPONENTHOST_INCLUDED
