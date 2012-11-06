/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PROXYMESSAGE_INCLUDED)
#define PROXYMESSAGE_INCLUDED

#include "Network/Message.h"

namespace Teardrop
{
	namespace Net
	{
		class ProxyMessage
			: public Message
		{
		public:
			unsigned int m_playerId;
			const static unsigned int MASK = 0x40000000;

			ProxyMessage(const Packet& packet);
			virtual ~ProxyMessage();

			// these are virtual for overriding by Raknet message classes
			void deserialize(RakNet::BitStream& bs);
			void serialize(RakNet::BitStream& bs);

			TD_DECLARE_ALLOCATOR();

		protected:
			ProxyMessage();
			virtual void _deserialize(RakNet::BitStream& bs) = 0;
			virtual void _serialize(RakNet::BitStream& bs) = 0;
		};
	}
}

#endif // PROXYMESSAGE_INCLUDED
