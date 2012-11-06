/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(UPDATEPLAYERSTATE_INCLUDED)
#define UPDATEPLAYERSTATE_INCLUDED

#include "Memory/Memory.h"
#include "Network/Message.h"
#include "Util/_String.h"

namespace Teardrop
{
	namespace Net
	{
		class UpdatePlayerState
			: public Message
		{
		public:
			TD_NETMESSAGE(UpdatePlayerState, MSGID_PLAYER_UPDATE_STATE);
			unsigned int m_id;
			int m_playerTeam;
			String m_playerName;
			String m_playerCharClass;
			unsigned int m_playerEntityVariant;
			unsigned short m_playerChar;
			bool m_bReady;

			UpdatePlayerState(const Packet& packet);
			virtual ~UpdatePlayerState();

			//! Message implementation
			void _deserialize(RakNet::BitStream& bs);
			void _serialize(RakNet::BitStream& bs);

			TD_DECLARE_ALLOCATOR();
		};
	}
}

#endif // UPDATEPLAYERSTATE_INCLUDED
