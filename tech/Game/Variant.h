/******************************************************************************
Copyright (c) 2015 Teardrop Games

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
