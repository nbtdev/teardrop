/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Variant.h"

using namespace Teardrop;

Variant::Variant()
{
}

Variant::Variant(const String& name, const String& targetActorName, const Reflection::ClassDef* actorClass)
{
	mName = name;
	mTarget = targetActorName;
	mClass = actorClass;
}

Variant::~Variant()
{
}

const String& Variant::name() const
{
	return mName;
}

const String& Variant::targetActorName() const
{
	return mTarget;
}

const Reflection::ClassDef* Variant::actorClass() const
{
	return mClass;
}

int Variant::numSlots() const
{
	SlotEquipment::const_reverse_iterator it = mSlotEquipment.rbegin();
	if (it != mSlotEquipment.rend())
		return it->first + 1;

	return 0;
}

int Variant::slotEquipment(int idx, const Mountable** equipList, int nEquipList) const
{
	int numEquipInSlot = mSlotEquipment.count(idx);

	if (!numEquipInSlot)
		return 0;

	if (!equipList || !nEquipList)
		return numEquipInSlot;

	int nEquip = std::min(nEquipList, numEquipInSlot);

	std::pair<SlotEquipment::const_iterator, SlotEquipment::const_iterator> pr = mSlotEquipment.equal_range(idx);
	for (int i=0; i<nEquip; ++i) {
		equipList[i] = pr.first->second;
		++(pr.first);
	}

	return nEquip;
}

int Variant::addEquipment(int slotIdx, const Mountable* equipment)
{
	const Mountable* tmp[] = {equipment};
	return addEquipment(slotIdx, tmp, 1);
}

int Variant::addEquipment(int slotIdx, const Mountable** equipment, int nEquipment)
{
	for (int i=0; i<nEquipment; ++i) {
		mSlotEquipment.insert(SlotEquipment::value_type(slotIdx, equipment[i]));
	}

	return mSlotEquipment.count(slotIdx);
}