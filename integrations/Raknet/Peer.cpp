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
#include "stdafx.h"
#include "Peer.h"
#include "Util/Hash.h"

using namespace Teardrop::Integration;

Raknet::Peer::Peer(RakNetGUID& guid)
: mGuid(guid)
{
}

Raknet::Peer::Peer(RakNetGUID& guid, SystemAddress& addr)
: mGuid(guid)
, mAddr(addr)
{
}

unsigned int Raknet::Peer::getPlayerId()
{
	return hashData(&mGuid, sizeof(mGuid));
}

#if 0
#include "NetworkSystem.h"
#include "Network/Message.h"
#include "Network/Messages/PingResponse.h"
#include "Network/Messages/ConnectionRequestAccepted.h"
#include "Network/Messages/ConnectionLost.h"
#include "Network/Messages/InterrogateServer.h"
#include "Util/_String.h"
#include "Util/Environment.h"
#include "Util/Logger.h"
#include "Util/Hash.h"


using namespace Teardrop::Integration;
using namespace Teardrop::Net;
using namespace RakNet;
//---------------------------------------------------------------------------
Raknet::Peer::Peer()
{
	m_pPeer = 0;
	g[0] = g[1] = g[2] = g[3] = 0;
	a = 0;
	p = 0;
	pad = 0;
}
//---------------------------------------------------------------------------
Raknet::Peer::~Peer()
{
}
//---------------------------------------------------------------------------
MessagePtr Raknet::Peer::getNextMessage()
{
	MessagePtr pMsg;
	Packet* pPacket = m_pPeer->Receive();

	// we only want to deal with TD messages here -- system/API messages
	// should be dealt with in a different path
	if (pPacket)
	{
#if defined(_DEBUG)
		char buf[64];
		sprintf_s(buf, 64, "[TD] Incoming packet: id=%d, len=%d", pPacket->data[0], pPacket->length);
		Environment::get().pLogger->logMessage(buf);
#endif
		BitStream bs(pPacket->data, pPacket->length, false);

		switch (pPacket->data[0])
		{
		case ID_NEW_INCOMING_CONNECTION:
			Environment::get().pLogger->logMessage("\nNEW INCOMING CONNECTION\n");
			break;

		case ID_USER_PACKET_ENUM+1:
			pMsg = NetworkSystem::createMessage(pPacket);
			if (pMsg)
				pMsg->deserialize(bs);
			break;

		case ID_PONG:
			pMsg = TD_NEW PingResponse(*pPacket);
			pMsg->deserialize(bs);
			break;

		case ID_CONNECTION_REQUEST_ACCEPTED:
			pMsg = TD_NEW ConnectionRequestAccepted(*pPacket);
			pMsg->deserialize(bs);
			break;

		case ID_CONNECTION_LOST:
		case ID_DISCONNECTION_NOTIFICATION:
			pMsg = TD_NEW ConnectionLost(*pPacket);
			pMsg->deserialize(bs);
			break;
		}
	}
	return pMsg;
}
//---------------------------------------------------------------------------
RemotePeer* Raknet::Peer::createRemotePeer(Message* pMsg)
{

}
//---------------------------------------------------------------------------
bool Raknet::Peer::connect(const String& addr, unsigned short port)
{
	return m_pPeer->Connect(addr, port, 0, 0);
}
//---------------------------------------------------------------------------
void Raknet::Peer::disconnect(RemotePeer* pPeer)
{
	// sending a zero to this method means disconnect ourselves
	if (pPeer)
		m_pPeer->CloseConnection((SystemAddress&)pPeer->a, true, 0, HIGH_PRIORITY);
	//else
	//	m_pPeer->CloseConnection(UNASSIGNED_SYSTEM_ADDRESS, true, 0, HIGH_PRIORITY);
	Sleep(500);
}
//---------------------------------------------------------------------------
void Raknet::Peer::disconnect(unsigned int addr, unsigned short port)
{
	m_pPeer->CloseConnection(
		SystemAddress(addr, port), 
		true, 0, HIGH_PRIORITY);
	Sleep(500);
}
//---------------------------------------------------------------------------
void Raknet::Peer::send(Message* pMsg, unsigned int* guid)
{
	SystemAddress dest;
	bool bBroadcast = (guid == 0);

	if (guid)
		dest = m_pPeer->GetSystemAddressFromGuid((RakNetGUID&)*guid);

	BitStream bs;
	pMsg->serialize(bs);

#if defined(_DEBUG)
	char buf[64];
	sprintf_s(buf, 64, "[TD] Outgoing packet: id=%s, len=%d", 
		NetworkSystem::getMessageString(pMsg->getId()), bs.GetWriteOffset());
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
void Raknet::Peer::send(Message* pMsg, const String& addr, size_t port)
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
void Raknet::Peer::ping(const char* addr, unsigned short port)
{
	m_pPeer->Ping(addr, port, false);
}
//---------------------------------------------------------------------------
void Raknet::Peer::setDisconnectedPingResponse(const char *data, unsigned int dataSize)
{
	m_pPeer->SetOfflinePingResponse(data, dataSize);
}
//---------------------------------------------------------------------------
// for unconnected systems (i.e available servers on the LAN)
void Raknet::Peer::requestServerInfo(const char* addr)
{
	//InterrogateServer msg;
	//send(&msg, addr, PORT_GAME_SERVER_CLIENT_LISTENER);
}
//---------------------------------------------------------------------------
bool Raknet::Peer::isLocalOrigination(Message* pMsg)
{
	return ((RakNetGUID&)g == (RakNetGUID&)pMsg->g);
}
//---------------------------------------------------------------------------
unsigned int Raknet::Peer::getId()
{
	return hashData(g, sizeof(g));
}
//---------------------------------------------------------------------------
unsigned int Raknet::Peer::getLocalIpV4()
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
#endif // 0
