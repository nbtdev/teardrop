/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(UPDATEPLAYERSTATE_INCLUDED)
#define UPDATEPLAYERSTATE_INCLUDED

#include "Network/Message.h"
#include "Network/Network.h"
#include "Network/Protocol.h"
#include "Memory/Memory.h"
#include "Util/_String.h"

namespace Teardrop
{
	namespace Net
	{
		class UpdatePlayerState
			: public Message
		{
		public:
			TD_NETMESSAGE(UpdatePlayerState, MSGID_PLAYER_UPDATE_STATE);
			unsigned int m_id;
			int m_playerTeam;
			String m_playerName;
			String m_playerCharClass;
			unsigned int m_playerEntityVariant;
			unsigned short m_playerChar;
			bool m_bReady;

			UpdatePlayerState();
			virtual ~UpdatePlayerState();

			//! Message implementation
			void deserialize(Net::Stream& bs);
			void serialize(Net::Stream& bs);

			TD_DECLARE_ALLOCATOR();
		};
	}
}

#endif // UPDATEPLAYERSTATE_INCLUDED
