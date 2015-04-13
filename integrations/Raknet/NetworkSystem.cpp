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
#include "NetworkSystem.h"
#include "Peer.h"
#include "Stream.h"
#include "Memory/Allocators.h"
#include "Util/Environment.h"
#include "Util/Logger.h"
#include "Util/_String.h"
#include "Network/Messages/ConnectionRequestAccepted.h"
#include "Network/Messages/PingResponse.h"
#include "Network/Messages/ConnectionLost.h"

using namespace Teardrop;
using namespace Integration;

Raknet::System::System(int maxIncomingConnections, short listenPort)
: mMe(0)
, mGuid(0)
{
	mMaxIncomingConnections = maxIncomingConnections;
	mListenPort = listenPort;
}

Raknet::System::~System()
{

}

Allocator* Raknet::System::s_pAllocator = TD_GET_ALLOCATOR(DEFAULT);

void* Raknet::System::malloc(size_t sz) 
{ 
	return s_pAllocator->Allocate(sz TD_ALLOC_SITE); 
}

void* Raknet::System::realloc(void* pMem, size_t sz) 
{ 
	return s_pAllocator->Reallocate(pMem, sz TD_ALLOC_SITE); 
}

void Raknet::System::free(void* pMem) 
{ 
	return s_pAllocator->Deallocate(pMem); 
}

void Raknet::System::setAllocator(Allocator* pAlloc)
{
	assert(pAlloc);
	s_pAllocator = pAlloc;
}

Allocator* Raknet::System::getAllocator()
{
	return s_pAllocator;
}

void Raknet::System::getTypes(Type* typeArray, int& typeCount)
{
	if (typeCount < 1)
	{
		typeCount = 0;
		return; // TODO: throw?
	}

	typeArray[0] = System::SYSTEM_NETWORK;
	typeCount = 1;
}

Net::Message* Raknet::System::createMessage(unsigned char msgId)
{
	// for now, the TD msg ID is still an unsigned char (tho this
	// is likely to change once we get lots of messages)
	if (s_factory[msgId])
		return s_factory[msgId]();

	return 0;
}

unsigned long long Raknet::System::getTime()
{
	return RakNet::GetTime();
}

bool Raknet::System::connect(const String& address, unsigned short port)
{
	return mMe->Connect(address, port, 0, 0);
}

Teardrop::Net::Peer* Raknet::System::createPeer(Net::Message& msg)
{
	Raknet::Peer* pPeer = static_cast<Raknet::Peer*>(msg.m_pPeer);
	return new Raknet::Peer(pPeer->mGuid);
}

void Raknet::System::destroyPeer(Teardrop::Net::Peer* pPeer)
{
	delete pPeer;
}

Net::MessagePtr Raknet::System::getNextMessage()
{
	Net::MessagePtr pMsg;
	Packet* pPacket = mMe->Receive();

	// we only want to deal with TD messages here -- system/API messages
	// should be dealt with in a different path
	if (pPacket)
	{
#if defined(_DEBUG)
		char buf[64];
		sprintf_s(buf, 64, "[TD] Incoming packet: id=%d, len=%d", pPacket->data[0], pPacket->length);
		Environment::get().pLogger->logMessage(buf);
#endif
		Raknet::Stream bs(pPacket->data, pPacket->length, false);

		// connected messages need to have their remote endpoint information set up
		bool bIsConnected = false;
		switch (pPacket->data[0])
		{
		// disconnected connection request, no guid or systemaddr available
		case ID_NEW_INCOMING_CONNECTION:
			Environment::get().pLogger->logMessage("\nNEW INCOMING CONNECTION\n");
			break;

		// Non-system/control messages have their message ID in the second byte
		case ID_USER_PACKET_ENUM+1:
			pMsg = createMessage(pPacket->data[1]);
			if (pMsg)
			{
				pMsg->deserialize(bs);
				bIsConnected = true;
			}
			break;

		// disconnected ping response, no guid or systemaddr available
		case ID_PONG:
			pMsg = TD_NEW Net::PingResponse;
			pMsg->deserialize(bs);
			break;

		// we sent out a connect request, and the other end replied in the affirmative;
		// this is where we set up the ID info for "the other guy", which should be extracted
		// by whoever processes this message and stored for later use in sending messages 
		// to this remote endpoint
		case ID_CONNECTION_REQUEST_ACCEPTED:
			pMsg = TD_NEW Net::ConnectionRequestAccepted;
			pMsg->deserialize(bs);
			bIsConnected = true;
			break;

		// connected disconnect notice, we need to 
		case ID_CONNECTION_LOST:
		case ID_DISCONNECTION_NOTIFICATION:
			pMsg = TD_NEW Net::ConnectionLost;
			pMsg->deserialize(bs);
			bIsConnected = true;
			break;
		}

		if (bIsConnected)
		{
			pMsg->m_pPeer = TD_NEW Raknet::Peer(pPacket->guid, pPacket->systemAddress);
		}
	}

	return pMsg;
}

// broadcast to all peers
void Raknet::System::send(Net::Message& msg)
{
	SystemAddress dest;

	Raknet::Stream bs;
	msg.serialize(bs);

#if defined(_DEBUG)
	char buf[64];
	sprintf_s(buf, 64, "[TD] Outgoing packet (broadcast): id=%s", NetworkSystem::getMessageString(msg.getId()));
	Environment::get().pLogger->logMessage(buf);
#endif
	mMe->Send(
		&bs.mBS,
		(PacketPriority)msg.m_priority,
		(PacketReliability)msg.m_reliability,
		char(msg.m_channel),
		dest, 
		true);
}

// send to single peer
void Raknet::System::send(Net::Message& msg, Teardrop::Net::Peer* pRecipient)
{
	Raknet::Stream bs;
	msg.serialize(bs);

	Raknet::Peer* pPeer = static_cast<Raknet::Peer*>(pRecipient);
	SystemAddress dest;
	if (pPeer)
		dest = mMe->GetSystemAddressFromGuid(pPeer->mGuid);
	else {
#if defined(_DEBUG)
		char buf[64];
		sprintf_s(buf, 64, "[TD] Failed to send outgoing packet (null recipient): id=%s", NetworkSystem::getMessageString(msg.getId()));
		Environment::get().pLogger->logMessage(buf);
#endif
		return;
	}

#if defined(_DEBUG)
	char buf[64];
	sprintf_s(buf, 64, "[TD] Outgoing packet: id=%s", NetworkSystem::getMessageString(msg.getId()));
	Environment::get().pLogger->logMessage(buf);
#endif
	mMe->Send(
		&bs.mBS,
		(PacketPriority)msg.m_priority,
		(PacketReliability)msg.m_reliability,
		char(msg.m_channel),
		dest, 
		false);
}

// send to multiple peers
void Raknet::System::send(Net::Message& msg, Teardrop::Net::Peer** pRecipients, int nRecipients)
{
	Raknet::Stream bs;
	msg.serialize(bs);

	for (int i=0; i<nRecipients; ++i) {
		Raknet::Peer* pPeer = static_cast<Raknet::Peer*>(pRecipients[i]);
		SystemAddress dest;
		if (pPeer)
			dest = mMe->GetSystemAddressFromGuid(pPeer->mGuid);
		else {
	#if defined(_DEBUG)
			char buf[64];
			sprintf_s(buf, 64, "[TD] Failed to send outgoing packet (null recipient): id=%s", NetworkSystem::getMessageString(msg.getId()));
			Environment::get().pLogger->logMessage(buf);
	#endif
			return;
		}

	#if defined(_DEBUG)
		char buf[64];
		sprintf_s(buf, 64, "[TD] Outgoing packet: id=%s", NetworkSystem::getMessageString(msg.getId()));
		Environment::get().pLogger->logMessage(buf);
	#endif
		mMe->Send(
			&bs.mBS,
			(PacketPriority)msg.m_priority,
			(PacketReliability)msg.m_reliability,
			char(msg.m_channel),
			dest, 
			false);
	}
}

void Raknet::System::disconnect(Net::Peer* pPeer)
{
	// sending a zero to this method means disconnect ourselves
	if (pPeer)
	{
		Raknet::Peer* pP = static_cast<Raknet::Peer*>(pPeer);
		mMe->CloseConnection(pP->mAddr, true, 0, HIGH_PRIORITY);
	}
	//else
	//	m_pPeer->CloseConnection(UNASSIGNED_SYSTEM_ADDRESS, true, 0, HIGH_PRIORITY);
	Sleep(500);
}

void Raknet::System::disconnect(unsigned int addr, unsigned short port)
{
	mMe->CloseConnection(
		SystemAddress(addr, port), 
		true, 0, HIGH_PRIORITY);
	Sleep(500);
}

// for unconnected systems (i.e servers in a server list)
void Raknet::System::ping(const char* addr, unsigned short port)
{
	mMe->Ping(addr, port, false);
}

void Raknet::System::setDisconnectedPingResponse(const char *data, unsigned int dataSize)
{
	mMe->SetOfflinePingResponse(data, dataSize);
}

// for unconnected systems (i.e available servers on the LAN)
void Raknet::System::requestServerInfo(const char* addr)
{
	//InterrogateServer msg;
	//send(&msg, addr, PORT_GAME_SERVER_CLIENT_LISTENER);
}

bool Raknet::System::isLocalOrigination(Net::Message* pMsg)
{
	Raknet::Peer* pPeer = static_cast<Raknet::Peer*>(pMsg->m_pPeer);
	return (mMe->GetGuidFromSystemAddress(UNASSIGNED_SYSTEM_ADDRESS) == pPeer->mGuid);
}

unsigned int Raknet::System::getLocalIpV4()
{
	const char * localIp = mMe->GetLocalIP(0);
	unsigned char ip[4];
	unsigned int a, b, c, d;
	sscanf_s(localIp, "%d.%d.%d.%d", &a, &b, &c, &d);
	ip[0] = d;
	ip[1] = c;
	ip[2] = b;
	ip[3] = a;
	return *((unsigned int*)ip);
}

#include "Network/Messages/Advertise.h"
#include "Network/Messages/Unadvertise.h"
#include "Network/Messages/PlayerJoinServer.h"
#include "Network/Messages/PlayerLeaveServer.h"
#include "Network/Messages/PlayerJoinGame.h"
#include "Network/Messages/PlayerLeaveGame.h"
#include "Network/Messages/UpdateServerState.h"
#include "Network/Messages/UpdatePlayerState.h"
#include "Network/Messages/GameStarted.h"
#include "Network/Messages/GameEnded.h"
#include "Network/Messages/PlayerPositionSync.h"
#include "Network/Messages/PlayerCommand.h"
#include "Network/Messages/InterrogateServer.h"
#include "Network/Messages/QueryIFF.h"
#include "Network/Messages/ResponseIFF.h"
#include "Network/Messages/PlayerEntityVariantChanged.h"

#include "RakMemoryOverride.h"
#include "PacketLogger.h"

class MyLogger : public PacketLogger
{
public:
	void WriteLog(const char* msg)
	{
#if defined(_DEBUG)
		Teardrop::Environment::get().pLogger->logMessage(msg);
#endif
	}
};
MyLogger s_logger;

using namespace Teardrop::Net;
using namespace Raknet;

void Raknet::System::initialize()
{
	rakMalloc = Raknet::System::malloc;
	rakRealloc = Raknet::System::realloc;
	rakFree = Raknet::System::free;

	Advertise _a;
	Unadvertise _u;
	PlayerJoinServer _pjs;
	PlayerLeaveServer _pls;
	PlayerJoinGame _pjg;
	PlayerLeaveGame _plg;
	UpdatePlayerState _ups;
	UpdateServerState _uss;
	GameStarted _gs;
	GameEnded _ge;
	PlayerPositionSync _pps;
	PlayerCommand _pc;
	InterrogateServer _ic;
	QueryIFF _qi;
	ResponseIFF _ri;
	PlayerVariantChanged _pvc;

	// initialize Raknet
	mMe = RakNetworkFactory::GetRakPeerInterface();
	mMe->AttachPlugin(&s_logger);

	// for now, prevent ping responses entirely (server can reset this later)
	mMe->SetOfflinePingResponse(0, 0);

	// startup our peer -- we talk to the master server as well as 
	// game clients through this peer, so make enough to go around
	SocketDescriptor desc(mListenPort, 0);
	bool rtn = mMe->Startup(mMaxIncomingConnections, 30, &desc, 1);
	if (rtn && mMaxIncomingConnections > 2)
	{
		mMe->SetMaximumIncomingConnections(mMaxIncomingConnections);
	}

	// set our guid and address members
	mGuid = new RakNetGUID;
	*mGuid = mMe->GetGuidFromSystemAddress(UNASSIGNED_SYSTEM_ADDRESS);
}

void Raknet::System::shutdown()
{
	delete mGuid;
	mGuid = 0;

	mMe->Shutdown(300);
	RakNetworkFactory::DestroyRakPeerInterface(mMe);
	mMe = 0;
}
