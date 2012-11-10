/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(QUERY_IFF_INCLUDED)
#define QUERY_IFF_INCLUDED

#include "Network/Message.h"
#include "Network/Network.h"
#include "Network/Protocol.h"
#include "Memory/Memory.h"
#include <vector>

namespace Teardrop
{
	namespace Net
	{
		class QueryIFF
			: public Message
		{
		public:
			TD_NETMESSAGE(QueryIFF, MSGID_QUERY_IFF);
			unsigned int m_selfId;
			std::vector<unsigned int> m_targetIds;

			QueryIFF();
			virtual ~QueryIFF();

			//! Message implementation
			void deserialize(Net::Stream& bs);
			void serialize(Net::Stream& bs);

			TD_DECLARE_ALLOCATOR();
		};
	}
}

#endif // QUERY_IFF_INCLUDED
