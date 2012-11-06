/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PLAYERLEAVEGAME_INCLUDED)
#define PLAYERLEAVEGAME_INCLUDED

#include "Memory/Memory.h"
#include "Network/Message.h"
#include "Util/_String.h"

namespace Teardrop
{
	namespace Net
	{
		class PlayerLeaveGame
			: public Message
		{
		public:
			TD_NETMESSAGE(PlayerLeaveGame, MSGID_PLAYER_LEAVE_GAME);
			unsigned int m_playerId;
			String m_additionalInfo;

			PlayerLeaveGame(const Packet& packet);
			virtual ~PlayerLeaveGame();

			//! Message implementation
			void _deserialize(RakNet::BitStream& bs);
			void _serialize(RakNet::BitStream& bs);

			TD_DECLARE_ALLOCATOR();
		};
	}
}

#endif // PLAYERLEAVEGAME_INCLUDED
