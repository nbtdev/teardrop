/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "stdafx.h"
#include "Message.h"
#include "Peer.h"
#include "Network.h"

//#if defined(_DEBUG)
#include "Util/Environment.h"
#include "Util/Logger.h"
//#endif

using namespace Teardrop;
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
	sprintf_s(buf, 128, "[TD Message] id: %d (%s)", 
		msgId, Network::getMessageString(msgId));
	Environment::get().pLogger->logMessage(buf);
//#endif
}
//---------------------------------------------------------------------------
void Message::serialize(BitStream& bs)
{
	// all TD messages use this RakNet message ID
	bs.Write(unsigned char(ID_USER_PACKET_ENUM+1));
	bs.Write(unsigned char(getId()));

	// the rest of the message is handled in a derived method
	_serialize(bs);

//#if defined(_DEBUG) // and "dump packet log"?
	char buf[128];
	sprintf_s(buf, 128, "[TD Message] id: %d (%s)", 
		getId(), Network::getMessageString(getId()));
	Environment::get().pLogger->logMessage(buf);
//#endif
}
