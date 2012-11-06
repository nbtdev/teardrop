/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PLAYERCOMMAND_INCLUDED)
#define PLAYERCOMMAND_INCLUDED

#include "Network/ProxyMessage.h"

namespace Teardrop
{
	namespace Net
	{
		class PlayerCommand
			: public ProxyMessage
		{
		public:
			TD_PROXYMESSAGE(PlayerCommand, MSGID_PLAYER_COMMAND);
			float extra[4]; // float and uint are both 32-bit
			unsigned char verb;

			PlayerCommand(const Packet& packet);
			virtual ~PlayerCommand();

			TD_DECLARE_ALLOCATOR();

		protected:
			void _deserialize(RakNet::BitStream& bs);
			void _serialize(RakNet::BitStream& bs);
		};
	}
}

#endif // PLAYERCOMMAND_INCLUDED
