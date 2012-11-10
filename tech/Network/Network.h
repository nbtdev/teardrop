/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
			virtual void send(Message& pMsg) = 0;
			virtual void disconnect(Peer* pPeer=0) = 0;
			//void disconnect(unsigned int addr, unsigned short port=0);
			virtual void ping(const char* address, unsigned short port) = 0;
			virtual void requestServerInfo(const char* address) = 0;
			virtual void setDisconnectedPingResponse(const char *data, unsigned int dataSize) = 0;
			virtual unsigned int getLocalIpV4() = 0;
			virtual bool isLocalOrigination(Message* pMsg) = 0;
			virtual uint64_t getTime() = 0;
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
