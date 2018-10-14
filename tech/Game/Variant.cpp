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

#include "Variant.h"
#include <algorithm>

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
    int numEquipInSlot = (int)mSlotEquipment.count(idx);

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

    return (int)mSlotEquipment.count(slotIdx);
}
