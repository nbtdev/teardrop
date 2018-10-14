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

#if !defined(PLAYERVARIANTCHANGED_INCLUDED)
#define PLAYERVARIANTCHANGED_INCLUDED

#include "Network/Message.h"
#include "Network/Network.h"
#include "Network/Protocol.h"
#include "Memory/Memory.h"
#include "Util/_String.h"
#include <list>

namespace Teardrop
{
	namespace Net
	{
		class PlayerVariantChanged
			: public Message
		{
		public:
			TD_NETMESSAGE(PlayerVariantChanged, MSGID_PLAYER_UPDATE_VARIANT);
			unsigned int mPlayerId;
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

			PlayerVariantChanged();
			virtual ~PlayerVariantChanged();

			TD_DECLARE_ALLOCATOR();

		protected:
			void deserialize(Net::Stream& bs);
			void serialize(Net::Stream& bs);
		};
	}
}

#endif // PLAYERVARIANTCHANGED_INCLUDED
