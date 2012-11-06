/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(GAMESTARTED_INCLUDED)
#define GAMESTARTED_INCLUDED

#include "Memory/Memory.h"
#include "Network/Message.h"

namespace Teardrop
{
	namespace Net
	{
		class GameStarted
			: public Message
		{
		public:
			TD_NETMESSAGE(GameStarted, MSGID_GAME_STARTED);
			unsigned int m_worldId;

			GameStarted(const Packet& packet);
			virtual ~GameStarted();

			//! Message implementation
			void _deserialize(RakNet::BitStream& bs);
			void _serialize(RakNet::BitStream& bs);

			TD_DECLARE_ALLOCATOR();
		};
	}
}

#endif // GAMESTARTED_INCLUDED
