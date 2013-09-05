/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(VARIANT_INCLUDED)
#define VARIANT_INCLUDED

#include <map>
#include "Util/_String.h"

namespace Teardrop
{
	namespace Reflection
	{
		class ClassDef;
	}

	class Mountable;

	class Variant
	{
		typedef std::multimap<int, const Mountable*> SlotEquipment;
		SlotEquipment mSlotEquipment;

		String mName;
		String mTarget;
		const Reflection::ClassDef* mClass;

	public:
		Variant();
		Variant(const String& name, const String& targetActorName, const Reflection::ClassDef* actorClass);
		virtual ~Variant();

		// variant name
		const String& name() const;
		// actor class to which this variant is restricted
		const Reflection::ClassDef* actorClass() const;
		// name of specific actor subtype to which this variant is further restricted
		const String& targetActorName() const;

		// number of slots defined in this variant
		int numSlots() const;
		// get list of equipment in slot; supply 0 (NULL) for equipList to get only count in slot; returns number of equipment in slot
		int slotEquipment(/*in*/ int idx, /*out*/ const Mountable** equipList, /*in*/ int nEquipList) const;

		// add single equipment to slot idx; returns number of equipment in slot after addition
		int addEquipment(/*in*/ int slotIdx, /*in*/ const Mountable* equipment);
		// add bulk equipment to slot idx; returns number of equipment in slot after addition
		int addEquipment(/*in*/ int slotIdx, /*in*/ const Mountable** equipmentList, /*in*/ int nEquipment);
	};
}

#endif // VARIANT_INCLUDED
