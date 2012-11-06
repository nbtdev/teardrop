/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Component_EquipmentSlot.h"
#include "ComponentHost.h"
#include <algorithm>

using namespace Teardrop;
//---------------------------------------------------------------------------
TD_CLASS_IMPL(EquipmentSlotComponent);
//---------------------------------------------------------------------------
EquipmentSlotComponent::EquipmentSlotComponent()
{
}
//---------------------------------------------------------------------------
EquipmentSlotComponent::~EquipmentSlotComponent()
{
}
//---------------------------------------------------------------------------
size_t EquipmentSlotComponent::getNumEquipment()
{
	return m_equipment.size();
}
//---------------------------------------------------------------------------
Mountable* EquipmentSlotComponent::getEquipment(size_t idx)
{
	if (idx >= m_equipment.size())
		return 0;

	return m_equipment[idx];
}
//---------------------------------------------------------------------------
void EquipmentSlotComponent::setEquipment(Mountable* pMountable, size_t idx)
{
	if (idx >= m_equipment.size())
		return;

	m_equipment[idx] = pMountable;
}
//---------------------------------------------------------------------------
bool EquipmentSlotComponent::addEquipment(Mountable* pMountable)
{
	// TODO: check against some sort of configured limit?

	m_equipment.push_back(pMountable);
	return true;
}
//---------------------------------------------------------------------------
Mountable* EquipmentSlotComponent::removeEquipment(size_t idx)
{
	if (idx >= m_equipment.size())
		return 0;

	Mountable* pMountable = m_equipment[idx];
	m_equipment[idx] = 0;
	return pMountable;
}
//---------------------------------------------------------------------------
bool EquipmentSlotComponent::hasEquipment(Mountable* pMountable)
{
	Mountables::iterator it = 
		std::find(m_equipment.begin(), m_equipment.end(), pMountable);
	return it != m_equipment.end();
}
//---------------------------------------------------------------------------
void EquipmentSlotComponent::removeEquipment(Mountable* pMountable)
{
	Mountables::iterator it = 
		std::find(m_equipment.begin(), m_equipment.end(), pMountable);

	if (it != m_equipment.end())
		*it = 0;
}
//---------------------------------------------------------------------------
const Transform& EquipmentSlotComponent::getParentTransformWS() const
{
	return m_pHost->getTransformWS();
}
