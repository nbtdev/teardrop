/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "Network.h"
#include "Protocol.h"
#include "Memory/Allocators.h"
#include <string.h>

using namespace Teardrop;
using namespace Net;
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
const char* NetworkSystem::getMessageString(unsigned int id)
{	
	if (id >= MSGID_NUM_MESSAGES)
		return "(undefined)";

	return s_pMessageStrings[id];
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
NetworkSystem::MessageCreateFn NetworkSystem::s_factory[MSGID_NUM_MESSAGES];
//---------------------------------------------------------------------------
NetworkSystem::NetworkSystem()
{
	m_pAllocator = TD_GET_ALLOCATOR(DEFAULT);
}

NetworkSystem::~NetworkSystem()
{

}