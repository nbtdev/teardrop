/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(GAMESTARTED_INCLUDED)
#define GAMESTARTED_INCLUDED

#include "Network/Message.h"
#include "Network/Network.h"
#include "Network/Protocol.h"
#include "Memory/Memory.h"

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

			GameStarted();
			virtual ~GameStarted();

			//! Message implementation
			void deserialize(Net::Stream& bs);
			void serialize(Net::Stream& bs);

			TD_DECLARE_ALLOCATOR();
		};
	}
}

#endif // GAMESTARTED_INCLUDED
