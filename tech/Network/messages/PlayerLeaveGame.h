/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PLAYERLEAVEGAME_INCLUDED)
#define PLAYERLEAVEGAME_INCLUDED

#include "Network/Message.h"
#include "Network/Network.h"
#include "Network/Protocol.h"
#include "Memory/Memory.h"
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
			unsigned int mPlayerId;
			String m_additionalInfo;

			PlayerLeaveGame();
			virtual ~PlayerLeaveGame();

			//! Message implementation
			void deserialize(Net::Stream& bs);
			void serialize(Net::Stream& bs);

			TD_DECLARE_ALLOCATOR();
		};
	}
}

#endif // PLAYERLEAVEGAME_INCLUDED
