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
