/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RAKNET_NETWORK_INTEGRATION_INCLUDED)
#define RAKNET_NETWORK_INTEGRATION_INCLUDED

#include "Network/Network.h"

class RakPeerInterface;
struct RakNetGUID;

namespace Teardrop
{
	class String;

	namespace Integration
	{
		namespace Raknet
		{
			class System : public Teardrop::Net::NetworkSystem
			{
				RakPeerInterface* mMe;
				RakNetGUID* mGuid;
				int mMaxIncomingConnections;
				short mListenPort;

			public:
				System(int maxIncomingConnections, short listenPort);
				~System();

				//! System implementation (partial)
				void setAllocator(Allocator* pAlloc);
				Allocator* getAllocator();
				void getTypes(System::Type* typeArray, int& typeCount);
				void initialize();
				void shutdown();

				// memory management functions, for Raknet memory mgmt overrides
				static void* malloc(size_t sz);
				static void* realloc(void* mem, size_t sz);
				static void free(void* pMem);
				static Allocator* s_pAllocator;

				//! Net::System implementation
				bool connect(const String& address, unsigned short port);
				Net::MessagePtr getNextMessage();
				// broadcast
				void send(Net::Message& msg);
				// send to single peer
				void send(Net::Message& msg, Net::Peer* mPeer);
				// send to list of peers
				void send(Net::Message& msg, Net::Peer** mPeers, int peerCount);
				void disconnect(Net::Peer* pPeer=0);
				void disconnect(unsigned int addr, unsigned short port=0);
				void ping(const char* address, unsigned short port);
				void requestServerInfo(const char* address);
				void setDisconnectedPingResponse(const char *data, unsigned int dataSize);
				unsigned int getLocalIpV4();
				bool isLocalOrigination(Net::Message* pMsg);
				unsigned long long getTime();
				Teardrop::Net::Peer* createPeer(Net::Message& msg);
				void destroyPeer(Teardrop::Net::Peer* pPeer);

			private:
				Net::Message* createMessage(unsigned char msgId);
			};
		}
	}
}

#endif // RAKNET_NETWORK_INTEGRATION_INCLUDED
