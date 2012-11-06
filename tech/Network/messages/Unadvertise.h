/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(UNADVERTISE_INCLUDED)
#define UNADVERTISE_INCLUDED

#include "Network/Message.h"
#include "Util/_String.h"

namespace Teardrop
{
	namespace Net
	{
		class Unadvertise
			: public Message
		{
		public:
			TD_NETMESSAGE(Unadvertise, MSGID_UNADVERTISE_SERVER);

			Unadvertise(const Packet& packet);
			virtual ~Unadvertise();

			//! Message implementation
			void _deserialize(RakNet::BitStream& bs);
			void _serialize(RakNet::BitStream& bs);

			TD_DECLARE_ALLOCATOR();
		};
	}
}

#endif // UNADVERTISE_INCLUDED
