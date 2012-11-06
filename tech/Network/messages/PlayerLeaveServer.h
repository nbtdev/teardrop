/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PLAYERLEAVESERVER_INCLUDED)
#define PLAYERLEAVESERVER_INCLUDED

#include "Memory/Memory.h"
#include "Network/Message.h"
#include "Util/_String.h"

namespace Teardrop
{
	namespace Net
	{
		class PlayerLeaveServer
			: public Message
		{
		public:
			TD_NETMESSAGE(PlayerLeaveServer, MSGID_PLAYER_LEAVE_SERVER);
			unsigned int m_id;

			PlayerLeaveServer(const Packet& packet);
			virtual ~PlayerLeaveServer();

			//! Message implementation
			void _deserialize(RakNet::BitStream& bs);
			void _serialize(RakNet::BitStream& bs);

			TD_DECLARE_ALLOCATOR();
		};
	}
}

#endif // PLAYERLEAVESERVER_INCLUDED
