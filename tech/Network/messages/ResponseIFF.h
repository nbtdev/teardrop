/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RESPONSE_IFF_INCLUDED)
#define RESPONSE_IFF_INCLUDED

#include "Network/Message.h"
#include "Network/Network.h"
#include "Network/Protocol.h"
#include "Memory/Memory.h"
#include <vector>

namespace Teardrop
{
	namespace Net
	{
		class ResponseIFF
			: public Message
		{
		public:

			struct Entry
			{
				unsigned int m_targetId;
				int m_iff;
			};

			TD_NETMESSAGE(ResponseIFF, MSGID_RESPONSE_IFF);
			unsigned int m_selfId;
			std::vector<Entry> m_iffInfo;

			ResponseIFF();
			virtual ~ResponseIFF();

			//! Message implementation
			void deserialize(Net::Stream& bs);
			void serialize(Net::Stream& bs);

			TD_DECLARE_ALLOCATOR();
		};
	}
}

#endif // RESPONSE_IFF_INCLUDED
