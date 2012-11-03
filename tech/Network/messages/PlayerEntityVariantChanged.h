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

#if !defined(PLAYERVARIANTCHANGED_INCLUDED)
#define PLAYERVARIANTCHANGED_INCLUDED

#include "Network/Message.h"
#include "Util/_String.h"
#include <list>

namespace CoS
{
	namespace Net
	{
		class PlayerVariantChanged
			: public Message
		{
		public:
			COS_NETMESSAGE(PlayerVariantChanged, MSGID_PLAYER_UPDATE_VARIANT);
			unsigned int m_playerId;
			unsigned short m_playerEntityId;
			int m_variantId;

			struct SlotConfig
			{
				unsigned int m_slotOrdinal;
				typedef std::list<String> SlotEquipment;
				SlotEquipment m_equipment;
			};
			typedef std::list<SlotConfig> SlotData;
			SlotData m_slotData;

			PlayerVariantChanged(const Packet& packet);
			virtual ~PlayerVariantChanged();

			COS_DECLARE_ALLOCATOR();

		protected:
			void _deserialize(RakNet::BitStream& bs);
			void _serialize(RakNet::BitStream& bs);
		};
	}
}

#endif // PLAYERVARIANTCHANGED_INCLUDED
