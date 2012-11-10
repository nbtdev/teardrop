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

			ProxyMessage();
			virtual ~ProxyMessage();

			// these are virtual for overriding by Raknet message classes
			void deserialize(Net::Stream& bs);
			void serialize(Net::Stream& bs);

			TD_DECLARE_ALLOCATOR();
		};
	}
}

#endif // PROXYMESSAGE_INCLUDED
