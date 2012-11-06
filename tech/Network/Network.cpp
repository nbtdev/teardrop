/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "stdafx.h"
#include "Network.h"
#include "Peer.h"
#include "Memory/Allocators.h"
#include "RakMemoryOverride.h"

using namespace Teardrop;
using namespace Net;
using namespace RakNet;
//---------------------------------------------------------------------------
DiscoveryData::DiscoveryData()
{
	memset(this, 0, sizeof(*this));
}
//---------------------------------------------------------------------------
static const char* s_pMessageStrings[] =
{
"MSGID_NULL",
"MSGID_ADVERTISE_SERVER",
"MSGID_UNADVERTISE_SERVER",
"MSGID_REQUEST_SERVER_LIST",
"MSGID_INTERROGATE_SERVER",
"MSGID_QUERY_IFF",
"MSGID_RESPONSE_IFF",
"MSGID_UPDATE_SERVER_INFO",
"MSGID_RESPOND_SERVER_LIST",
"MSGID_PLAYER_JOIN_SERVER",
"MSGID_PLAYER_UPDATE_STATE",
"MSGID_PLAYER_UPDATE_VARIANT",
"MSGID_PLAYER_LEAVE_SERVER",
"MSGID_PLAYER_JOIN_GAME",
"MSGID_PLAYER_LEAVE_GAME",
"MSGID_GAME_STARTED",
"MSGID_GAME_ENDED",
"MSGID_CHAT_MESSAGE",
"MSGID_PROXY_MESSAGE_FLAG",
"MSGID_PLAYER_POSITION_SYNC",
"MSGID_PLAYER_COMMAND",
};
//---------------------------------------------------------------------------
const char* Network::getMessageString(unsigned int id)
{	
	if (id >= MSGID_NUM_MESSAGES)
		return "(undefined)";

	return s_pMessageStrings[id];
}
//---------------------------------------------------------------------------
Allocator* Network::s_pAllocator = TD_GET_ALLOCATOR(DEFAULT);
void* Network::malloc(size_t sz) 
{ 
	return s_pAllocator->Allocate(sz TD_ALLOC_SITE); 
}
void* Network::realloc(void* pMem, size_t sz) 
{ 
	return s_pAllocator->Reallocate(pMem, sz TD_ALLOC_SITE); 
}
void Network::free(void* pMem) 
{ 
	return s_pAllocator->Deallocate(pMem); 
}
//---------------------------------------------------------------------------
Network::MessageCreateFn Network::s_factory[MSGID_NUM_MESSAGES];
//---------------------------------------------------------------------------
void Network::setAllocator(Allocator* pAlloc)
{
	assert(pAlloc);
	s_pAllocator = pAlloc;
}
//---------------------------------------------------------------------------
Message* Network::createMessage(const Packet* packet)
{
	// for now, the TD msg ID is still an unsigned char (tho this
	// is likely to change once we get lots of messages)
	if (s_factory[packet->data[1]])
		return s_factory[packet->data[1]](packet);

	return 0;
}
//---------------------------------------------------------------------------
Peer* Network::createPeer()
{
	return TD_NEW Peer;
}
//---------------------------------------------------------------------------
void Network::destroyPeer(Peer* pPeer)
{
	delete pPeer;
}
//---------------------------------------------------------------------------
#include "messages/Advertise.h"
#include "messages/Unadvertise.h"
#include "messages/PlayerJoinServer.h"
#include "messages/PlayerLeaveServer.h"
#include "messages/PlayerJoinGame.h"
#include "messages/PlayerLeaveGame.h"
#include "messages/UpdateServerState.h"
#include "messages/UpdatePlayerState.h"
#include "messages/GameStarted.h"
#include "messages/GameEnded.h"
#include "messages/PlayerPositionSync.h"
#include "messages/PlayerCommand.h"
#include "messages/InterrogateServer.h"
#include "messages/QueryIFF.h"
#include "messages/ResponseIFF.h"
#include "messages/PlayerEntityVariantChanged.h"
void Network::initialize()
{
	rakMalloc = Network::malloc;
	rakRealloc = Network::realloc;
	rakFree = Network::free;

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
}
