/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
