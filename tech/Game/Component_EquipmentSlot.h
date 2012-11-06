/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(EQUIPMENTSLOT_INCLUDED)
#define EQUIPMENTSLOT_INCLUDED

#include "Game/Component.h"
#include "Math/Matrix44.h"

namespace Teardrop
{
	class Mountable;
	class Transform;

	class EquipmentSlotComponent
		: public Component
	{
		typedef std::vector<Mountable*> Mountables;
		Mountables m_equipment;

	public:
		TD_CLASS(EquipmentSlotComponent, Component);
		TD_PROPERTY(ID, "Unique ID (on this avatar) of this slot", int, -1, 0);
		TD_PROPERTY(MountPoint, "Mount point associated with this equipment slot", String, "", 0);
		TD_PROPERTY(Size, "Maximum equipment capacity of this slot", int, 1, 0);

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
		
		TD_DECLARE_ALLOCATOR();

	protected:
		Matrix44 m_transform;
	};
}

#endif // EQUIPMENTSLOT_INCLUDED
