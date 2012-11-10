/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "PlayerEntityVariantChanged.h"
#include "Stream.h"

using namespace Teardrop;
using namespace Net;

//---------------------------------------------------------------------------
TD_NETMESSAGE_IMPL(PlayerVariantChanged);
//---------------------------------------------------------------------------
PlayerVariantChanged::PlayerVariantChanged()
{
}
//---------------------------------------------------------------------------
PlayerVariantChanged::~PlayerVariantChanged()
{
}
//---------------------------------------------------------------------------
void PlayerVariantChanged::deserialize(Net::Stream& bs)
{
	bs.read(m_playerId);
	bs.read(m_playerEntityId);
	bs.read(m_variantId);

	int sz;
	bs.read(sz);

	for (int i=0; i<sz; ++i)
	{
		m_slotData.push_back(SlotConfig());
		SlotConfig& data = m_slotData.back();

		// get the ordinal of this slot
		bs.read(data.m_slotOrdinal);

		// then get the number of equipment in the slot
		unsigned int numEquip;
		bs.read(numEquip);

		// and then read the name of each piece of equipment
		for (unsigned int j=0; j<numEquip; ++j)
		{
			String str;
			bs.read(str);
			data.m_equipment.push_back(str);
		}
	}
}
//---------------------------------------------------------------------------
void PlayerVariantChanged::serialize(Net::Stream& bs)
{
	bs.write(m_playerId);
	bs.write(m_playerEntityId);
	bs.write(m_variantId);

	int sz = int(m_slotData.size());
	bs.write(sz);

	for (SlotData::iterator it = m_slotData.begin(); it != m_slotData.end(); ++it)
	{
		// write the slot ordinal number
		bs.write(it->m_slotOrdinal);

		// write the number of equipment pieces in the slot
		int numEquip = int(it->m_equipment.size());
		bs.write(numEquip);

		// then write the name of each piece of equipment there
		for (SlotConfig::SlotEquipment::iterator e = it->m_equipment.begin();
			e != it->m_equipment.end(); ++e)
		{
			bs.write(*e);
		}
	}
}
