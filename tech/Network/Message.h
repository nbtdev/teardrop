/******************************************************************************
Copyright (c) 2015 Teardrop Games

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
******************************************************************************/

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
