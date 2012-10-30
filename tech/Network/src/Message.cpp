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
#include "Message.h"
#include "Peer.h"
#include "Network.h"

//#if defined(_DEBUG)
#include "Util/include/Environment.h"
#include "Util/include/Logger.h"
//#endif

using namespace CoS;
using namespace Net;
using namespace RakNet;
//---------------------------------------------------------------------------
Message::Message(const Packet& packet)
{
	g[0] = 0;
	g[1] = 0;
	g[2] = 0;
	g[3] = 0;

	// extract the routing information
	(RakNetGUID&)g = packet.guid;
	(SystemAddress&)a = packet.systemAddress;
	m_priority = MEDIUM_PRIORITY;
	m_reliability = RELIABLE;
	m_channel = 0;
}
//---------------------------------------------------------------------------
Message::Message()
{
	m_priority = MEDIUM_PRIORITY;
	m_reliability = RELIABLE;
	m_channel = 0;
}
//---------------------------------------------------------------------------
Message::~Message()
{
}
//---------------------------------------------------------------------------
void Message::deserialize(BitStream& bs)
{
	// handled entirely in a derived method (we already know the message
	// ID because of the subclass that was created for this message)
	unsigned char id, msgId;
	bs.Read(id);
	bs.Read(msgId);
	_deserialize(bs);

//#if defined(_DEBUG) // and "dump packet log"?
	char buf[128];
	sprintf_s(buf, 128, "[CoS Message] id: %d (%s)", 
		msgId, Network::getMessageString(msgId));
	Environment::get().pLogger->logMessage(buf);
//#endif
}
//---------------------------------------------------------------------------
void Message::serialize(BitStream& bs)
{
	// all CoS messages use this RakNet message ID
	bs.Write(unsigned char(ID_USER_PACKET_ENUM+1));
	bs.Write(unsigned char(getId()));

	// the rest of the message is handled in a derived method
	_serialize(bs);

//#if defined(_DEBUG) // and "dump packet log"?
	char buf[128];
	sprintf_s(buf, 128, "[CoS Message] id: %d (%s)", 
		getId(), Network::getMessageString(getId()));
	Environment::get().pLogger->logMessage(buf);
//#endif
}
