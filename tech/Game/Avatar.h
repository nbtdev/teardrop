/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(AVATAR_INCLUDED)
#define AVATAR_INCLUDED

#include "Game/ZoneObject.h"
#include "Game/IPlayerControlled.h"
#include "Math/Matrix44.h"

/*
	Avatar is meant to be a base class for actors (MACRO, Aero, Infantry, etc).
	It handles jobs such as mount-point management. 
*/

namespace Teardrop
{
	class Mountable;
	class EquipmentSlotComponent;

	class Avatar 
		: public ZoneObject
		, public IPlayerControlled
	{
	public:
		TD_CLASS(Avatar, ZoneObject);

		Avatar();
		virtual ~Avatar();

		//! Reflection::Object overrides
		bool initialize();
		bool destroy();
		bool update(float deltaT);

		//! add all of a variant's equipment to the avatar
		bool equip(unsigned int variantId);
		//! add a variant's equipment to the avatar, piecewise (this is 
		//! used mostly by the server); returns false if a piece of equipment
		//! cannot be found or does not fit in the indicated slot
		bool equip(unsigned int slot, const String& equipmentName);

		//! IPlayerControlled implementation
		void issueCommand(Command& command);

		TD_DECLARE_ALLOCATOR();

	protected:
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
		virtual void _issueCommand(Command& command);

		// request all weapons assigned to the indicated weapon group, to fire
		void fireWeaponGroup(int weapon);

		//! remove all mounted equipment
		void clearAllSlots();
		//! put the Mountable in the indicated slot
		bool addToSlot(Mountable* pMountable, int slotId);

		/*
			mount point management data structures and methods
		*/
		struct MountPoint
		{
			Matrix44 m_transform;
			size_t m_index; 
			size_t m_boneIndex;
			EquipmentSlotComponent* m_pSlot;
		};

		typedef std::map<String, MountPoint> MountPoints;
		MountPoints m_mountPoints;

		// these happen when an avatar has a variant set on it before
		// the mount points have been initialized; we need to store them
		// for later and mount them when we can
		struct DelayedMount
		{
			Mountable* pMountable;
			int slotId;
		};
		typedef std::vector<DelayedMount> DelayedMounts;
		DelayedMounts m_delayedMounts;

		//! internal helper method
		bool equip(unsigned int slot, const String& equipmentName, Reflection::ClassDef* pClassDef);
	};
}

#endif // AVATAR_INCLUDED
