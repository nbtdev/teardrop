/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(NETMESSAGE_INCLUDED)
#define NETMESSAGE_INCLUDED

#include "Memory/Memory.h"
#include "Network/Network.h"
#include "Util/SharedPointer.h"

struct Packet;
namespace RakNet
{
	class BitStream;
}

namespace Teardrop
{
	namespace Net
	{
		class Message
		{
		public:
			int m_priority;
			int m_reliability;
			int m_channel;
			unsigned int g[4]; // guid
			unsigned int a;    // IPv4 address
			unsigned short p;
			unsigned short pad;

			Message(const Packet& packet);
			virtual ~Message();

			virtual size_t getId() = 0;

			// these are virtual for overriding by Raknet message classes
			virtual void deserialize(RakNet::BitStream& bs);
			virtual void serialize(RakNet::BitStream& bs);

			TD_DECLARE_ALLOCATOR();

		protected:
			Message();
			virtual void _deserialize(RakNet::BitStream& bs) = 0;
			virtual void _serialize(RakNet::BitStream& bs) = 0;
		};

		typedef SharedPointer<Message> MessagePtr;
	}
}

#endif // NETMESSAGE_INCLUDED
