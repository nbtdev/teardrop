/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(CHATMESSAGE_INCLUDED)
#define CHATMESSAGE_INCLUDED

#include "Network/Message.h"
#include "Util/_String.h"

namespace Teardrop
{
	namespace Net
	{
		class ChatMessage
			: public Message
		{
		public:
			TD_NETMESSAGE(ChatMessage, MSGID_CHAT_MESSAGE);
			// the chat message, clear text
			String m_message;
			// distribution control -- (-1) means "broadcast in the open"
			int m_team;
			// sender ID
			unsigned int m_senderId;
			// message source
			int m_source;

			ChatMessage(const Packet& packet);
			virtual ~ChatMessage();

			//! Message implementation
			void _deserialize(RakNet::BitStream& bs);
			void _serialize(RakNet::BitStream& bs);

			TD_DECLARE_ALLOCATOR();
		};
	}
}

#endif // CHATMESSAGE_INCLUDED
