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
#include "stdafx.h"
#include "PlayerEntityVariantChanged.h"

using namespace CoS;
using namespace Net;
using namespace RakNet;
//---------------------------------------------------------------------------
COS_NETMESSAGE_IMPL(PlayerVariantChanged);
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
