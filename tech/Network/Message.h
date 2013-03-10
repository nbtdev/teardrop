/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(NET_MESSAGE_INCLUDED)
#define NET_MESSAGE_INCLUDED

#include "MessageFwd.h"

namespace Teardrop
{
	namespace Net
	{
		class Stream;
		class Peer;

		enum MessagePriority
		{
			PRIORITY_SYSTEM,
			PRIORITY_HIGH,
			PRIORITY_MEDIUM,
			PRIORITY_LOW,

			PRIORITY_TYPE_COUNT
		};

		enum MessageReliability
		{
			RELIABILITY_UNRELIABLE,
			RELIABILITY_UNRELIABLE_SEQUENCED,
			RELIABILITY_RELIABLE,
			RELIABILITY_ORDERED,
			RELIABILITY_RELIABILE_SEQUENCED,

			RELIABILITY_TYPE_COUNT
		};

		enum MessageClass
		{
			MESSAGE_SYSTEM,
			MESSAGE_USER,
			MESSAGE_OOB,

			MESSAGE_TYPE_COUNT
		};

		class Message
		{
		public:
			// used only for incoming messages
			Peer* m_pPeer; 

			int m_channel;
			MessagePriority m_priority;
			MessageReliability m_reliability;
			MessageClass m_class;

			Message();
			virtual ~Message();

			virtual size_t getId() = 0;
			virtual bool isProxyMessage();

			// these are virtual for overriding by subclasses
			virtual void deserialize(Stream& s) = 0;
			virtual void serialize(Stream& s) = 0;
		};
	}
}

#endif // NET_MESSAGE_INCLUDED
