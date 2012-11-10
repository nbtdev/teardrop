/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(GAMEENDED_INCLUDED)
#define GAMEENDED_INCLUDED

#include "Network/Message.h"
#include "Network/Network.h"
#include "Network/Protocol.h"
#include "Memory/Memory.h"

namespace Teardrop
{
	namespace Net
	{
		class GameEnded
			: public Message
		{
		public:
			TD_NETMESSAGE(GameEnded, MSGID_GAME_ENDED);

			GameEnded();
			virtual ~GameEnded();

			//! Message implementation
			void deserialize(Net::Stream& bs);
			void serialize(Net::Stream& bs);

			TD_DECLARE_ALLOCATOR();
		};
	}
}

#endif // GAMEENDED_INCLUDED
