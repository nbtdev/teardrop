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
#include "stdafx.h"
#include "Peer.h"
#include "RemotePeer.h"
#include "Network.h"
#include "Message.h"
#include "messages/PingResponse.h"
#include "messages/ConnectionRequestAccepted.h"
#include "messages/ConnectionLost.h"
#include "messages/InterrogateServer.h"
#include "Util/include/_String.h"
#include "Util/include/Environment.h"
#include "Util/include/Logger.h"
#include "Util/include/Hash.h"
#include "PacketLogger.h"
class MyLogger : public PacketLogger
{
public:
	void WriteLog(const char* msg)
	{
#if defined(_DEBUG)
		CoS::Environment::get().pLogger->logMessage(msg);
#endif
	}
};
MyLogger s_logger;

using namespace CoS;
using namespace Net;
using namespace RakNet;
//---------------------------------------------------------------------------
Peer::Peer()
{
	memset(this, 0, sizeof(Peer));
}
//---------------------------------------------------------------------------
Peer::~Peer()
{
}
//---------------------------------------------------------------------------
MessagePtr Peer::getNextMessage()
{
	MessagePtr pMsg;
	Packet* pPacket = m_pPeer->Receive();

	// we only want to deal with CoS messages here -- system/API messages
	// should be dealt with in a different path
	if (pPacket)
	{
#if defined(_DEBUG)
		char buf[64];
		sprintf_s(buf, 64, "[COS] Incoming packet: id=%d, len=%d", pPacket->data[0], pPacket->length);
		Environment::get().pLogger->logMessage(buf);
#endif
		BitStream bs(pPacket->data, pPacket->length, false);

		switch (pPacket->data[0])
		{
		case ID_NEW_INCOMING_CONNECTION:
			Environment::get().pLogger->logMessage("\nNEW INCOMING CONNECTION\n");
			break;

		case ID_USER_PACKET_ENUM+1:
			pMsg = Network::createMessage(pPacket);
			if (pMsg)
				pMsg->deserialize(bs);
			break;

		case ID_PONG:
			pMsg = COS_NEW PingResponse(*pPacket);
			pMsg->deserialize(bs);
			break;

		case ID_CONNECTION_REQUEST_ACCEPTED:
			pMsg = COS_NEW ConnectionRequestAccepted(*pPacket);
			pMsg->deserialize(bs);
			break;

		case ID_CONNECTION_LOST:
		case ID_DISCONNECTION_NOTIFICATION:
			pMsg = COS_NEW ConnectionLost(*pPacket);
			pMsg->deserialize(bs);
			break;
		}
	}
	return pMsg;
}
//---------------------------------------------------------------------------
bool Peer::startup(size_t maxIncomingConnections, unsigned short port)
{
	// initialize Raknet
	m_pPeer = RakNetworkFactory::GetRakPeerInterface();
	m_pPeer->AttachPlugin(&s_logger);

	// for now, prevent ping responses entirely (server can reset this later)
	m_pPeer->SetOfflinePingResponse(0, 0);

	// startup our peer -- we talk to the master server as well as 
	// game clients through this peer, so make enough to go around
	SocketDescriptor desc(port, 0);
	bool rtn = m_pPeer->Startup(maxIncomingConnections, 30, &desc, 1);
	if (rtn && maxIncomingConnections > 2)
	{
		m_pPeer->SetMaximumIncomingConnections(maxIncomingConnections);
	}

	// set our guid and address members
	((RakNetGUID&)g) = m_pPeer->GetGuidFromSystemAddress(UNASSIGNED_SYSTEM_ADDRESS);

	return rtn;
}
//---------------------------------------------------------------------------
void Peer::shutdown()
{
	m_pPeer->Shutdown(300);
	RakNetworkFactory::DestroyRakPeerInterface(m_pPeer);
	m_pPeer = 0;
}
//---------------------------------------------------------------------------
RemotePeer* Peer::createRemotePeer(Message* pMsg)
{
	if (pMsg->getId() != ConnectionRequestAccepted::ID)
		return 0;

	ConnectionRequestAccepted* pCRA = 
		static_cast<ConnectionRequestAccepted*>(pMsg);

	RemotePeer* pRemote = COS_NEW RemotePeer;
	((SystemAddress&)pRemote->a) = ((SystemAddress&)pCRA->a);
	((RakNetGUID&)*pRemote->g) = ((RakNetGUID&)*pCRA->g);
	pRemote->m_pPeer = m_pPeer;
	return pRemote;
}
//---------------------------------------------------------------------------
bool Peer::connect(const String& addr, unsigned short port)
{
	return m_pPeer->Connect(addr, port, 0, 0);
}
//---------------------------------------------------------------------------
void Peer::disconnect(RemotePeer* pPeer)
{
	// sending a zero to this method means disconnect ourselves
	if (pPeer)
		m_pPeer->CloseConnection((SystemAddress&)pPeer->a, true, 0, HIGH_PRIORITY);
	//else
	//	m_pPeer->CloseConnection(UNASSIGNED_SYSTEM_ADDRESS, true, 0, HIGH_PRIORITY);
	Sleep(500);
}
//---------------------------------------------------------------------------
void Peer::disconnect(unsigned int addr, unsigned short port)
{
	m_pPeer->CloseConnection(
		SystemAddress(addr, port), 
		true, 0, HIGH_PRIORITY);
	Sleep(500);
}
//---------------------------------------------------------------------------
void Peer::send(Message* pMsg, unsigned int* guid)
{
	SystemAddress dest;
	bool bBroadcast = (guid == 0);

	if (guid)
		dest = m_pPeer->GetSystemAddressFromGuid((RakNetGUID&)*guid);

	BitStream bs;
	pMsg->serialize(bs);

#if defined(_DEBUG)
	char buf[64];
	sprintf_s(buf, 64, "[COS] Outgoing packet: id=%s, len=%d", 
		Network::getMessageString(pMsg->getId()), bs.GetWriteOffset());
	Environment::get().pLogger->logMessage(buf);
#endif
	m_pPeer->Send(
		&bs,
		(PacketPriority)pMsg->m_priority,
		(PacketReliability)pMsg->m_reliability,
		0,//char(pMsg->m_channel),
		dest, 
		bBroadcast);
}
//---------------------------------------------------------------------------
void Peer::send(Message* pMsg, const String& addr, size_t port)
{
	SystemAddress dest;
	dest.SetBinaryAddress(addr);
	dest.port = unsigned short(port);

	BitStream bs;
	pMsg->serialize(bs);

	m_pPeer->Send(
		&bs,
		(PacketPriority)pMsg->m_priority,
		(PacketReliability)pMsg->m_reliability,
		0,//char(pMsg->m_channel),
		dest, 
		false);
}
//---------------------------------------------------------------------------
// for unconnected systems (i.e servers in a server list)
void Peer::ping(const char* addr, unsigned short port)
{
	m_pPeer->Ping(addr, port, false);
}
//---------------------------------------------------------------------------
void Peer::setDisconnectedPingResponse(const char *data, unsigned int dataSize)
{
	m_pPeer->SetOfflinePingResponse(data, dataSize);
}
//---------------------------------------------------------------------------
// for unconnected systems (i.e available servers on the LAN)
void Peer::requestServerInfo(const char* addr)
{
	//InterrogateServer msg;
	//send(&msg, addr, PORT_GAME_SERVER_CLIENT_LISTENER);
}
//---------------------------------------------------------------------------
bool Peer::isLocalOrigination(Message* pMsg)
{
	return ((RakNetGUID&)g == (RakNetGUID&)pMsg->g);
}
//---------------------------------------------------------------------------
unsigned int Peer::getId()
{
	return hashData(g, sizeof(g));
}
//---------------------------------------------------------------------------
unsigned int Peer::getLocalIpV4()
{
	const char * localIp = m_pPeer->GetLocalIP(0);
	unsigned char ip[4];
	unsigned int a, b, c, d;
	sscanf_s(localIp, "%d.%d.%d.%d", &a, &b, &c, &d);
	ip[0] = d;
	ip[1] = c;
	ip[2] = b;
	ip[3] = a;
	return *((unsigned int*)ip);
}
