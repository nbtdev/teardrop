/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "stdafx.h"
#include "PlayerEntityVariantChanged.h"

using namespace Teardrop;
using namespace Net;
using namespace RakNet;
//---------------------------------------------------------------------------
TD_NETMESSAGE_IMPL(PlayerVariantChanged);
//---------------------------------------------------------------------------
PlayerVariantChanged::PlayerVariantChanged(const Packet& packet) 
: Message(packet)
{
}
//---------------------------------------------------------------------------
PlayerVariantChanged::~PlayerVariantChanged()
{
}
//---------------------------------------------------------------------------
void PlayerVariantChanged::_deserialize(RakNet::BitStream& bs)
{
	bs.Read(m_playerId);
	bs.Read(m_playerEntityId);
	bs.Read(m_variantId);

	int sz;
	bs.Read(sz);

	for (int i=0; i<sz; ++i)
	{
		m_slotData.push_back(SlotConfig());
		SlotConfig& data = m_slotData.back();

		// get the ordinal of this slot
		bs.Read(data.m_slotOrdinal);

		// then get the number of equipment in the slot
		unsigned int numEquip;
		bs.Read(numEquip);

		// and then read the name of each piece of equipment
		for (unsigned int j=0; j<numEquip; ++j)
		{
			RakString str;
			bs.Read(str);
			data.m_equipment.push_back(String(str));
		}
	}
}
//---------------------------------------------------------------------------
void PlayerVariantChanged::_serialize(BitStream& bs)
{
	bs.Write(m_playerId);
	bs.Write(m_playerEntityId);
	bs.Write(m_variantId);

	int sz = int(m_slotData.size());
	bs.Write(sz);

	for (SlotData::iterator it = m_slotData.begin(); it != m_slotData.end(); ++it)
	{
		// write the slot ordinal number
		bs.Write(it->m_slotOrdinal);

		// write the number of equipment pieces in the slot
		int numEquip = int(it->m_equipment.size());
		bs.Write(numEquip);

		// then write the name of each piece of equipment there
		for (SlotConfig::SlotEquipment::iterator e = it->m_equipment.begin();
			e != it->m_equipment.end(); ++e)
		{
			RakString str(*e);
			bs.Write(str);
		}
	}
}
