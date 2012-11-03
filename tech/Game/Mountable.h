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

#if !defined(MOUNTABLE_INCLUDED)
#define MOUNTABLE_INCLUDED

#include "Game/ComponentHost.h"
#include "Gfx/IMeshInstanceProvider.h"
#include "Util/_String.h"
#include "Reflection/Reflection.h"
#include "Memory/Allocators.h"

namespace CoS
{
	/*
		A Mountable object generally is a set of cooperative components
		that implement the object's behavior: mesh and VFX rendering, 
		SFX, physics, etc. It's very similar to a ZoneObject except that
		it does not exist on its own in a Zone; it exists only as an 
		object attached to a host.
	*/

	class Transform;
	class EquipmentSlotComponent;
	class GfxRenderer;

	class Mountable
		: public Reflection::Object
		, public ComponentHost
		, public IMeshInstanceProvider
	{
	public:
		COS_CLASS(Mountable, Object);

		Mountable();
		virtual ~Mountable();
		
		//! Reflection::Object overrides; derived classes can override these
		//! or implement the protected methods below
		bool initialize();
		bool destroy();

		//! IComponentHost implementation; derived classes do not
		//! need to override these
		const Transform& getTransformWS();
		void setBoundingBox(const AABB& aabb);
		bool isOfType(Reflection::ClassDef*);

		bool update(float deltaT);

		//! return true if this object is mounted, false otherwise
		bool isMounted() const;

		//! called by the mount host when this object is mounted
		virtual void notifyMounted(EquipmentSlotComponent*);
		//! called by the mount host when this object is un-mounted
		virtual void notifyUnmounted();
		//! called by the mount host to collect any renderable mesh instances
		//! (IMeshInstanceProvider implementation)
		virtual void queueForRendering(GfxRenderer* pRend);

		COS_DECLARE_ALLOCATOR();


	protected:
		EquipmentSlotComponent* m_pSlot;

		/*
			these will be called from the overrides above; derived classes
			can either override these methods or override the ones above - it 
			depends on whether or not the derived class implementor (a) needs
			to execute in a different order, or (b) doesn't like explicit base-
			class method calls.
		*/
		virtual bool _initialize();
		virtual bool _destroy();
		virtual bool _update(float deltaT);
	};
}

#endif // MOUNTABLE_INCLUDED
