/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PINGRESPONSE_INCLUDED)
#define PINGRESPONSE_INCLUDED

#include "Network/Message.h"
#include "Network/Types.h"
#include "Memory/Memory.h"

namespace Teardrop
{
	namespace Net
	{
		class PingResponse
			: public Message
		{
		public:
			static const unsigned int ID = (0x80000000 | 26); // ID_PONG -- fix this hack someday
			uint64_t m_time;
			DiscoveryData m_data;

			PingResponse();
			virtual ~PingResponse();
			size_t getId() { return ID; }

			//! Message implementation
			void deserialize(Net::Stream& bs);
			void serialize(Net::Stream& bs);

			TD_DECLARE_ALLOCATOR();
		};
	}
}

#endif // PINGRESPONSE_INCLUDED
