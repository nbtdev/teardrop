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

#if !defined(RAKNET_PEER_INCLUDED)
#define RAKNET_PEER_INCLUDED

#include "Network/Peer.h"
#include "RakNetTypes.h"

namespace Teardrop
{
	namespace Integration
	{
		namespace Raknet
		{
			class Peer : public Net::Peer
			{
			public:
				Peer(RakNetGUID& guid);
				Peer(RakNetGUID& guid, SystemAddress& addr);

				RakNetGUID mGuid;
				SystemAddress mAddr;

				unsigned int getPlayerId();

#if 0
				bool startup(size_t maxIncomingConnections, unsigned short port);
				void shutdown();

				void disconnect(Net::RemotePeer* pPeer=0);
				void disconnect(unsigned int addr, unsigned short port=0);
				void ping(const char* address, unsigned short port);
				void requestServerInfo(const char* address);
				void setDisconnectedPingResponse(const char *data, unsigned int dataSize);
				unsigned int getLocalIpV4();

				Net::MessagePtr getNextMessage();
				void send(Net::Message* pMsg, unsigned int* guid=0 /*0=broadcast*/);
				void send(Net::Message* pMsg, const String& addr, size_t port=size_t(-1));

				bool isLocalOrigination(Net::Message* pMsg);

				TD_DECLARE_ALLOCATOR();

			protected:
				RakPeerInterface* m_pPeer;
				unsigned int g[4];
				unsigned int a;
				unsigned short p;
				unsigned short pad;
#endif // 0
			};
		}
	}
}

#endif // RAKNET_PEER_INCLUDED
