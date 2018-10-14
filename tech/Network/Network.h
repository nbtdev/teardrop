/******************************************************************************
Copyright (c) 2018 Teardrop Games

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

#if !defined(NETWORK_INCLUDED)
#define NETWORK_INCLUDED

#include "Util/System.h"
#include "Network/MessageFwd.h"
#include "Network/Types.h"

namespace Teardrop
{
	class Allocator;
	class String;

	namespace Net
	{
		class Peer;

		/*
			Core networking interface class
		*/
		class NetworkSystem : public System
		{
			Allocator* m_pAllocator;

		protected:
			NetworkSystem();
			virtual ~NetworkSystem();

		public:
			virtual bool connect(const String& address, unsigned short port) = 0;
			virtual Teardrop::Net::MessagePtr getNextMessage() = 0;
			// broadcast
			virtual void send(Message& msg) = 0;
			// send to single peer
			virtual void send(Message& msg, Peer* mPeer) = 0;
			// send to list of peers
			virtual void send(Message& msg, Peer** mPeers, int peerCount) = 0;
			virtual void disconnect(Peer* pPeer=0) = 0;
			//void disconnect(unsigned int addr, unsigned short port=0);
			virtual void ping(const char* address, unsigned short port) = 0;
			virtual void requestServerInfo(const char* address) = 0;
			virtual void setDisconnectedPingResponse(const char *data, unsigned int dataSize) = 0;
			virtual unsigned int getLocalIpV4() = 0;
			virtual bool isLocalOrigination(Message* pMsg) = 0;
			virtual unsigned long long getTime() = 0;
			virtual Peer* createPeer(Message& msg) = 0;
			virtual void destroyPeer(Peer* pPeer) = 0;
			typedef Message* (*MessageCreateFn)();
			static MessageCreateFn s_factory[];

			//! map a message ID to its human-readable string
			static const char* getMessageString(unsigned int id);
		};

	}
}

//
//// handy defines for use in message definitions
//#define TD_NETMESSAGE(c) \
//	static Teardrop::Net::Message* __create(const Packet*); \
//	static const size_t ID; \
//	size_t getId() { return ID; } \
//	c() : Message() {} \
//	static struct MessageInitializer \
//	{ \
//		MessageInitializer(size_t id) \
//		{ \
//			c _c; \
//			Teardrop::Net::Network::s_factory[id] = __create; \
//		} \
//	} s_messageInit; \


//#define TD_NETMESSAGE_IMPL(c, id) \
//	const size_t c::ID = id; \
//	Teardrop::Net::Message* Teardrop::Net::c::__create(const Packet* packet) { return TD_NEW c(*packet); } \
//	Teardrop::Net::c::MessageInitializer Teardrop::Net::c::s_messageInit(id); \

// handy defines for use in message definitions
#define TD_NETMESSAGE(c, id) \
	static Teardrop::Net::Message* __create(); \
	static const size_t ID = id; \
	size_t getId() { return ID; } \
	static struct MessageInitializer \
{ \
	MessageInitializer() \
{ \
	c _c; \
	Teardrop::Net::NetworkSystem::s_factory[id] = __create; \
} \
} s_messageInit; \

#define TD_NETMESSAGE_IMPL(c) \
	Teardrop::Net::Message* Teardrop::Net::c::__create() { return TD_NEW c; } \
	Teardrop::Net::c::MessageInitializer Teardrop::Net::c::s_messageInit; \

#define TD_PROXYMESSAGE(c, id) \
	static Teardrop::Net::Message* __create(); \
	static const size_t ID = id; \
	size_t getId() { return ID; } \
	static struct MessageInitializer \
{ \
	MessageInitializer() \
{ \
	c _c; \
	Teardrop::Net::NetworkSystem::s_factory[id] = __create; \
} \
} s_messageInit; \

#define TD_PROXYMESSAGE_IMPL(c) \
	Teardrop::Net::Message* Teardrop::Net::c::__create() { return TD_NEW c; } \
	Teardrop::Net::c::MessageInitializer Teardrop::Net::c::s_messageInit; \

#endif // NETWORK_INCLUDED
