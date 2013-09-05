/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(MOUNTABLE_INCLUDED)
#define MOUNTABLE_INCLUDED

#include "Game/ComponentHost.h"
#include "Gfx/IMeshInstanceProvider.h"
#include "Util/_String.h"
#include "Reflection/Reflection.h"
#include "Memory/Allocators.h"

namespace Teardrop
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
		TD_CLASS(Mountable, Object);
		TD_PROPERTY(Name, "Mountable Equipment Name", String, "", 0);

		Mountable();
		virtual ~Mountable();
		
		//! Reflection::Object overrides; derived classes can override these
		//! or implement the protected methods below
		bool initialize();
		bool destroy();
		Reflection::Object* clone() const;

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

		TD_DECLARE_ALLOCATOR();


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
