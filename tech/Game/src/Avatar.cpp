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

#include "Avatar.h"
#include "Component_Render.h"
#include "Component_Animation.h"
#include "Component_CameraInterface.h"
#include "Component_EquipmentSlot.h"
#include "Mountable.h"
#include "Manifest.h"
#include "ObjectLibrary.h"
#include "Weapon.h"
#include "Animation/include/AnimationBlender.h"
#include "Math/include/Matrix44.h"
#include "Util/include/Environment.h"
#include "Util/include/Logger.h"

using namespace CoS;
//---------------------------------------------------------------------------
COS_CLASS_IMPL(Avatar);
//---------------------------------------------------------------------------
Avatar::Avatar()
{
}
//---------------------------------------------------------------------------
Avatar::~Avatar()
{
}
//---------------------------------------------------------------------------
bool Avatar::initialize()
{
	if (ZoneObject::initialize())
		return _initialize();

	return false;
}
//---------------------------------------------------------------------------
bool Avatar::destroy()
{
	_destroy();
	setObjectId(0);
	return ZoneObject::destroy();
}
//---------------------------------------------------------------------------
bool Avatar::update(float deltaT)
{
	if (_update(deltaT))
	{
		// we can't extract mount points from the derived avatar type
		// until it's fully initialized, so we need to do this here in 
		// update() as opposed to in initialize()

		// check components for an AnimationComponent
		AnimationComponent* pAnim = 0;
		if (findComponents(AnimationComponent::getClassDef(), (Component**)&pAnim))
		{
			// and then, if we have a valid animation blender...
			AnimationBlender* pBlender = pAnim->getBlender();
			if (pBlender)
			{
				// and then, if it has mounts but our mount data is empty...
				size_t numMP = pBlender->getNumMountPoints();
				if (numMP)
				{
					if (m_mountPoints.size() == 0)
					{
						// then we can fill in our data structure
						for (size_t i=0; i<numMP; ++i)
						{
							const AnimationBlender::MountPointData* pData = 
								pBlender->getMountPoint(i);

							MountPoint mp;
							mp.m_index = i;
							mp.m_boneIndex = pData->m_boneIndex;
							mp.m_transform = pData->m_transform;
							mp.m_pSlot = 0;
							m_mountPoints[pData->m_name] = mp;
						}

						// now get a list of all of the EquipmentSlotComponents 
						// configured for us; we need to associate each component 
						// with a mount point
						ComponentList list;
						findComponents(EquipmentSlotComponent::getClassDef(), list);
						for (ComponentList::iterator it=list.begin(); it != list.end(); ++it)
						{
							EquipmentSlotComponent* pSlot = static_cast<EquipmentSlotComponent*>(*it);
							MountPoints::iterator mp = m_mountPoints.find(pSlot->getMountPoint());
							if (mp != m_mountPoints.end())
								mp->second.m_pSlot = pSlot;
							else
							{
								// log a mismatch
								String msg("Could not find mount point '");
								msg += pSlot->getMountPoint();
								msg += String("' in avatar: ");
								msg += getName();

								Environment::get().pLogger->logMessage(msg);
							}
						}

						// and finally, clear up our mount backlog, if any
						for (size_t i=0; i<m_delayedMounts.size(); ++i)
						{
							addToSlot(m_delayedMounts[i].pMountable, m_delayedMounts[i].slotId);
						}
						m_delayedMounts.clear();
					}
					else
					{
						// we can update our mount point transforms
						for (size_t i=0; i<numMP; ++i)
						{
							const AnimationBlender::MountPointData* pData = 
								pBlender->getMountPoint(i);

							MountPoints::iterator it = m_mountPoints.find(pData->m_name);
							if (it != m_mountPoints.end())
							{
								it->second.m_transform = pData->m_transform;
								it->second.m_pSlot->setTransform(pData->m_transform);
							}
						}
					}
				}
			}
		}

		// update our mounted objects
		for (MountPoints::iterator it = m_mountPoints.begin();
			it != m_mountPoints.end(); ++it)
		{
			EquipmentSlotComponent* pComp = it->second.m_pSlot;
			if (pComp)
			{
				size_t nEquip = pComp->getNumEquipment();
				for (size_t j=0; j<nEquip; ++j)
				{
					Mountable* pMounted = pComp->getEquipment(j);
					if (pMounted)
						pMounted->update(deltaT);
				}
			}
		}

		// Avatars might have camera components too
		CameraInterfaceComponent* pCamComp;
		if (findComponents(
			CameraInterfaceComponent::getClassDef(), (Component**)&pCamComp))
		{
			pCamComp->update(deltaT);
		}

		return ZoneObject::update(deltaT);
	}

	return false;
}
//---------------------------------------------------------------------------
void Avatar::issueCommand(Command& cmd)
{
	_issueCommand(cmd);
}
//---------------------------------------------------------------------------
void Avatar::clearAllSlots()
{
	RenderComponent* pRend = 0;
	findComponents(RenderComponent::getClassDef(), (Component**)&pRend);

		// clear all mountables from all slots on all mount points
	for (MountPoints::iterator it = m_mountPoints.begin(); 
		it != m_mountPoints.end(); ++it)
	{
		EquipmentSlotComponent* pSlot = it->second.m_pSlot;
		if (pSlot)
		{
			size_t numEquipment = pSlot->getNumEquipment();
			for (size_t j=0; j<numEquipment; ++j)
			{
				Mountable* pMountable = pSlot->removeEquipment(j);

				// also, need to unregister the Mountable with any RenderComponent
				// we have (for now, just register with the first one found, if any)
				if (pRend)
					pRend->removeMeshInstanceProvider(pMountable);

				// then finally, destroy and delete it
				if (pMountable)
				{
					pMountable->notifyUnmounted();
					pMountable->destroy();
				}
				delete pMountable;
			}
		}
	}
}
//---------------------------------------------------------------------------
bool Avatar::addToSlot(Mountable* pMountable, int slotId)
{
	// first, check to see if we have mount points; if not, store this off
	// in a delayed mount list
	if (!m_mountPoints.size())
	{
		DelayedMount delayed;
		delayed.pMountable = pMountable;
		delayed.slotId = slotId;
		m_delayedMounts.push_back(delayed);
		return true;
	}

	// look through the mount points (since it's cheaper to iterate this 
	// set than it is to look up all of the slot components) for a slot that
	// has this ID; if not found, return false
	for (MountPoints::iterator it = m_mountPoints.begin(); 
		it != m_mountPoints.end(); ++it)
	{
		EquipmentSlotComponent* pSlot = it->second.m_pSlot;
		if (pSlot && pSlot->getID() == slotId)
		{
			if (pSlot->addEquipment(pMountable))
			{
				pMountable->notifyMounted(pSlot);

				// also, need to register the Mountable with any RenderComponent
				// we have (for now, just register with the first one found, if any)
				RenderComponent* pRend = 0;
				if (findComponents(RenderComponent::getClassDef(), (Component**)&pRend))
				{
					pRend->addMeshInstanceProvider(pMountable);
				}

				return true;
			}
		}
	}

	return false;
}
//---------------------------------------------------------------------------
bool Avatar::equip(
	unsigned int slot, 
	const String& equipmentName, 
	Reflection::ClassDef* pClassDef)
{
	Manifest* pManifest = Environment::get().pManifest;

	// first look up the equipment in the database
	const Manifest::ComponentHost* pCompHost = 
		pManifest->findComponentHost(
		pClassDef,
		equipmentName);

	// if it doesn't exist, error out
	if (!pCompHost)
		return false;

	// otherwise, make one
	Mountable* pObj = dynamic_cast<Mountable*>(
		pManifest->createInstance(pCompHost));

	// if we made one, and it's the expected type...
	if (pObj)
	{
		// ... initialize it...
		if (pObj->initialize())
		{
			// ...and (try to) add it to our loadout
			if (addToSlot(pObj, slot))
				return true;

			// failure usually means "slot full" or "wrong 
			// type of equipment for this slot". We need to
			// destroy and delete our equipment instance...
			pObj->destroy();
			delete pObj;
		}
	}

	// otherwise, return false
	return false;
}
//---------------------------------------------------------------------------
bool Avatar::equip(unsigned int slot, const String& equipmentName)
{
	Manifest* pManifest = Environment::get().pManifest;
	const Manifest::NVP& nvp = pManifest->getDefinitions();
	Manifest::NVP::const_iterator it = nvp.find(equipmentName);

	// if we can't find the definition, the equipment doesn't exist
	if (it == nvp.end())
		return false;

	return equip(slot, equipmentName, it->second.pClassDef);
}
//---------------------------------------------------------------------------
bool Avatar::equip(unsigned int variantId)
{
	Manifest* pManifest = Environment::get().pManifest;
	const Manifest::Variant* pVariant = pManifest->findVariant(variantId);

	if (!pVariant)
		return false;

	// first, clear everything we may already have on us
	clearAllSlots();

	// then give us everything that this variant wants
	const Manifest::Slots& slots = pVariant->m_slots;
	for (size_t s=0; s<slots.size(); ++s)
	{
		for (size_t e=0; e<slots[s].m_equipment.size(); ++e)
		{
			// EquipmentList is really just a list of names; look up each 
			// name in the manifest to make an instance of one
			const Manifest::ManifestEntry* pEntry = slots[s].m_equipment[e].m_pEntry;
			if (pEntry)
			{
				if (!equip(
					slots[s].m_ordinal, 
					slots[s].m_equipment[e].m_name, 
					pEntry->pClassDef))
				{
					clearAllSlots();
					return false;
				}
			}
		}
	}

	return true;
}
//---------------------------------------------------------------------------
bool Avatar::_initialize()
{
	return true;
}
//---------------------------------------------------------------------------
bool Avatar::_destroy()
{
	return true;
}
//---------------------------------------------------------------------------
bool Avatar::_update(float /*deltaT*/)
{
	return true;
}
//---------------------------------------------------------------------------
void Avatar::_issueCommand(Command& /*cmd*/)
{
}
//---------------------------------------------------------------------------
void Avatar::fireWeaponGroup(int /*groupId*/)
{
	// for now, fire everything
	ComponentList list;
	findComponents(EquipmentSlotComponent::getClassDef(), list);
	for (ComponentList::iterator it = list.begin(); it != list.end(); ++it)
	{
		EquipmentSlotComponent* pSlot = static_cast<EquipmentSlotComponent*>(*it);
		if (pSlot)
		{
			size_t numEquipment = pSlot->getNumEquipment();
			for (size_t i=0; i<numEquipment; ++i)
			{
				Mountable* pMountable = pSlot->getEquipment(i);
				if (pMountable && pMountable->isOfType(Weapon::getClassDef()))
				{
					Weapon* pWeapon = static_cast<Weapon*>(pMountable);
					pWeapon->fire();
				}
			}
		}
	}
}
