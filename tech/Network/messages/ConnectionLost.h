/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(CONNECTIONLOST_INCLUDED)
#define CONNECTIONLOST_INCLUDED

#include "Network/Message.h"
#include "Memory/Memory.h"

namespace Teardrop
{
	namespace Net
	{
		class ConnectionLost
			: public Message
		{
		public: 
			static const unsigned int ID = (0x80000000 | 20); // ID_CONNECTION_LOST -- fix this hack someday
			unsigned int m_playerId;

			ConnectionLost();
			virtual ~ConnectionLost();
			size_t getId() { return ID; }

			//! Message implementation
			void deserialize(Net::Stream& bs);
			void serialize(Net::Stream& bs);

			TD_DECLARE_ALLOCATOR();
		};
	}
}

#endif // CONNECTIONLOST_INCLUDED
