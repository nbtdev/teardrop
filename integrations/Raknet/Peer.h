/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
