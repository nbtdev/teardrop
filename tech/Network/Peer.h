/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(NETPEER_INCLUDED)
#define NETPEER_INCLUDED

#include "Memory/Memory.h"
#include "Network/Message.h"

class RakPeerInterface;

namespace Teardrop
{
	class String;

	namespace Net
	{
		class Message;
		class RemotePeer;

		class Peer
		{
		public:
			Peer();
			~Peer();

			unsigned int getId();

			bool startup(size_t maxIncomingConnections, unsigned short port);
			void shutdown();

			bool connect(const String& address, unsigned short port);
			RemotePeer* createRemotePeer(Message* pMsg); // should be ConnectionRequestAccepted message
			void disconnect(RemotePeer* pPeer=0);
			void disconnect(unsigned int addr, unsigned short port=0);
			void ping(const char* address, unsigned short port);
			void requestServerInfo(const char* address);
			void setDisconnectedPingResponse(const char *data, unsigned int dataSize);
			unsigned int getLocalIpV4();

			MessagePtr getNextMessage();
			void send(Message* pMsg, unsigned int* guid=0 /*0=broadcast*/);
			void send(Message* pMsg, const String& addr, size_t port=size_t(-1));

			bool isLocalOrigination(Message* pMsg);

			TD_DECLARE_ALLOCATOR();

		protected:
			RakPeerInterface* m_pPeer;
			unsigned int g[4];
			unsigned int a;
			unsigned short p;
			unsigned short pad;
		};
	}
}

#endif // NETPEER_INCLUDED
