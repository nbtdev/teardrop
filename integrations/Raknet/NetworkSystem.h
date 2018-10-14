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
