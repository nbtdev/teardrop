/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(ADVERTISE_INCLUDED)
#define ADVERTISE_INCLUDED

#include "Network/Message.h"
#include "Util/_String.h"

namespace Teardrop
{
	namespace Net
	{
		class Advertise
			: public Message
		{
		public:
			TD_NETMESSAGE(Advertise, MSGID_ADVERTISE_SERVER);
			String m_serverName;
			String m_mapName;

			Advertise(const Packet& packet);
			virtual ~Advertise();

			//! Message implementation
			void _deserialize(RakNet::BitStream& bs);
			void _serialize(RakNet::BitStream& bs);

			TD_DECLARE_ALLOCATOR();
		};
	}
}

#endif // ADVERTISE_INCLUDED
