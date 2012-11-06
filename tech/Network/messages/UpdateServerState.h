/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(UPDATESERVERSTATE_INCLUDED)
#define UPDATESERVERSTATE_INCLUDED

#include "Network/Message.h"
#include "Util/_String.h"

namespace Teardrop
{
	namespace Net
	{
		class UpdateServerState
			: public Message
		{
		public:
			TD_NETMESSAGE(UpdateServerState, MSGID_UPDATE_SERVER_INFO);
			unsigned int m_ip;
			int m_maxNumTeams;
			String m_serverName;
			String m_mapName;
			int m_mapId;
			size_t m_numPlayers;
			size_t m_ping;
			unsigned char m_updateOrRemove; // 0 for update, 1 for remove
			bool m_bInProgress;

			UpdateServerState(const Packet& packet);
			virtual ~UpdateServerState();

			//! Message implementation
			void _deserialize(RakNet::BitStream& bs);
			void _serialize(RakNet::BitStream& bs);

			TD_DECLARE_ALLOCATOR();
		};
	}
}

#endif // UPDATESERVERSTATE_INCLUDED
