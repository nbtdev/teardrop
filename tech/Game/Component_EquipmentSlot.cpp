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
