/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PLAYERJOINSERVER_INCLUDED)
#define PLAYERJOINSERVER_INCLUDED

#include "Memory/Memory.h"
#include "Network/Message.h"
#include "Util/_String.h"

namespace Teardrop
{
	namespace Net
	{
		class PlayerJoinServer
			: public Message
		{
		public:
			TD_NETMESSAGE(PlayerJoinServer, MSGID_PLAYER_JOIN_SERVER);
			unsigned int m_id;
			String m_playerName;
			String m_playerCharClass;
			unsigned short m_playerChar;
			unsigned int m_playerEntityVariant;

			PlayerJoinServer(const Packet& packet);
			virtual ~PlayerJoinServer();

			//! Message implementation
			void _deserialize(RakNet::BitStream& bs);
			void _serialize(RakNet::BitStream& bs);

			TD_DECLARE_ALLOCATOR();
		};
	}
}

#endif // PLAYERJOINSERVER_INCLUDED
