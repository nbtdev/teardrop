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
	bs.read(mPlayerId);
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
	bs.write(mPlayerId);
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
