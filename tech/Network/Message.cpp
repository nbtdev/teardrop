/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "Message.h"
#include "Network.h"
#include "Stream.h"

//#if defined(_DEBUG)
#include "Util/Environment.h"
#include "Util/Logger.h"
//#endif

using namespace Teardrop;
using namespace Net;
//---------------------------------------------------------------------------
Message::Message()
{
	m_priority = PRIORITY_MEDIUM;
	m_reliability = RELIABILITY_RELIABLE;
	m_class = MESSAGE_USER;
	m_channel = 0;
	m_pPeer = 0;
}
//---------------------------------------------------------------------------
Message::~Message()
{
	delete m_pPeer;
}
#if 0
//---------------------------------------------------------------------------
void Message::deserialize(Net::Stream& s)
{
	// handled entirely in a derived method (we already know the message
	// ID because of the subclass that was created for this message)
	unsigned char id, msgId;
	s.read(id);
	s.read(msgId);
	deserialize(s);

//#if defined(_DEBUG) // and "dump packet log"?
	char buf[128];
	sprintf_s(buf, 128, "[TD Message] id: %d (%s)", 
		msgId, NetworkSystem::getMessageString(msgId));
	Environment::get().pLogger->logMessage(buf);
//#endif
}
//---------------------------------------------------------------------------
void Message::serialize(Net::Stream& s)
{
	// all TD messages use this RakNet message ID
	s.write(unsigned char(ID_USER_PACKET_ENUM+1));
	s.write(unsigned char(getId()));

	// the rest of the message is handled in a derived method
	serialize(s);

//#if defined(_DEBUG) // and "dump packet log"?
	char buf[128];
	sprintf_s(buf, 128, "[TD Message] id: %d (%s)", 
		getId(), NetworkSystem::getMessageString(getId()));
	Environment::get().pLogger->logMessage(buf);
//#endif
}
#endif // 0