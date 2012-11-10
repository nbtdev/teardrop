/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PLAYERJOINGAME_INCLUDED)
#define PLAYERJOINGAME_INCLUDED

#include "Network/Message.h"
#include "Network/Network.h"
#include "Network/Protocol.h"
#include "Memory/Memory.h"
#include "Util/_String.h"

namespace Teardrop
{
	namespace Net
	{
		class PlayerJoinGame
			: public Message
		{
		public:
			TD_NETMESSAGE(PlayerJoinGame, MSGID_PLAYER_JOIN_GAME);
			unsigned int m_id;			// player global unique ID
			unsigned int m_avatarId;	// ID of this avatar in its class
			String m_avatarClass;		// class of the avatar
			String m_playerName;		// player's display name

			PlayerJoinGame();
			virtual ~PlayerJoinGame();

			//! Message implementation
			void deserialize(Net::Stream& bs);
			void serialize(Net::Stream& bs);

			TD_DECLARE_ALLOCATOR();
		};
	}
}

#endif // PLAYERJOINGAME_INCLUDED
