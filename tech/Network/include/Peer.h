/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

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
-----------------------------------------------------------------------------
*/

#if !defined(NETPEER_INCLUDED)
#define NETPEER_INCLUDED

#include "Memory/include/Memory.h"
#include "Network/include/Message.h"

class RakPeerInterface;

namespace CoS
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

			COS_DECLARE_ALLOCATOR();

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
