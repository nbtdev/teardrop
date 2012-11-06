/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(GAMEENDED_INCLUDED)
#define GAMEENDED_INCLUDED

#include "Memory/Memory.h"
#include "Network/Message.h"

namespace Teardrop
{
	namespace Net
	{
		class GameEnded
			: public Message
		{
		public:
			TD_NETMESSAGE(GameEnded, MSGID_GAME_ENDED);

			GameEnded(const Packet& packet);
			virtual ~GameEnded();

			//! Message implementation
			void _deserialize(RakNet::BitStream& bs);
			void _serialize(RakNet::BitStream& bs);

			TD_DECLARE_ALLOCATOR();
		};
	}
}

#endif // GAMEENDED_INCLUDED
