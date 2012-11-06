/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(INTERROGATE_SERVER_INCLUDED)
#define INTERROGATE_SERVER_INCLUDED

#include "Network/Message.h"

namespace Teardrop
{
	namespace Net
	{
		class InterrogateServer
			: public Message
		{
		public:
			TD_NETMESSAGE(InterrogateServer, MSGID_INTERROGATE_SERVER);
			unsigned int m_addr;

			InterrogateServer(const Packet& packet);
			virtual ~InterrogateServer();

			//! Message implementation
			void _deserialize(RakNet::BitStream& bs);
			void _serialize(RakNet::BitStream& bs);

			TD_DECLARE_ALLOCATOR();
		};
	}
}

#endif // INTERROGATE_SERVER_INCLUDED
