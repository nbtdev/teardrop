/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RAKNET_NETWORK_INTEGRATION_INCLUDED)
#define RAKNET_NETWORK_INTEGRATION_INCLUDED

#include "Network/Network.h"

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
				RakNetGUID mGuid;
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
				void send(Net::Message& pMsg);
				void disconnect(Net::Peer* pPeer=0);
				void disconnect(unsigned int addr, unsigned short port=0);
				void ping(const char* address, unsigned short port);
				void requestServerInfo(const char* address);
				void setDisconnectedPingResponse(const char *data, unsigned int dataSize);
				unsigned int getLocalIpV4();
				bool isLocalOrigination(Net::Message* pMsg);
				uint64_t getTime();

			private:
				Net::Message* createMessage(unsigned char msgId);
			};
		}
	}
}

#endif // RAKNET_NETWORK_INTEGRATION_INCLUDED
