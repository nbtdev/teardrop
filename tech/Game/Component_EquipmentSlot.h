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

#if !defined(EQUIPMENTSLOT_INCLUDED)
#define EQUIPMENTSLOT_INCLUDED

#include "Game/Component.h"
#include "Math/Matrix44.h"

namespace CoS
{
	class Mountable;
	class Transform;

	class EquipmentSlotComponent
		: public Component
	{
		typedef std::vector<Mountable*> Mountables;
		Mountables m_equipment;

	public:
		COS_CLASS(EquipmentSlotComponent, Component);
		COS_PROPERTY(ID, "Unique ID (on this avatar) of this slot", int, -1, 0);
		COS_PROPERTY(MountPoint, "Mount point associated with this equipment slot", String, "", 0);
		COS_PROPERTY(Size, "Maximum equipment capacity of this slot", int, 1, 0);

		EquipmentSlotComponent();
		~EquipmentSlotComponent();

		size_t getNumEquipment();
		Mountable* getEquipment(size_t idx);
		void setEquipment(Mountable* pMountable, size_t idx);
		bool addEquipment(Mountable* pMountable);
		bool hasEquipment(Mountable* pMountable);
		Mountable* removeEquipment(size_t idx);
		void removeEquipment(Mountable* pMountable);

		void setTransform(const Matrix44& xform) { m_transform = xform; }
		const Matrix44& getTransform() { return m_transform; }

		const Transform& getParentTransformWS() const;
		
		COS_DECLARE_ALLOCATOR();

	protected:
		Matrix44 m_transform;
	};
}

#endif // EQUIPMENTSLOT_INCLUDED
