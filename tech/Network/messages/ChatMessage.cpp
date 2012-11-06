/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "stdafx.h"
#include "ChatMessage.h"

using namespace Teardrop;
using namespace Net;
using namespace RakNet;
//---------------------------------------------------------------------------
TD_NETMESSAGE_IMPL(ChatMessage);
//---------------------------------------------------------------------------
ChatMessage::ChatMessage(const Packet& packet) : Message(packet)
{
	m_team = 0; // default to "broadcast"
}
//---------------------------------------------------------------------------
ChatMessage::~ChatMessage()
{
}
//---------------------------------------------------------------------------
void ChatMessage::_deserialize(RakNet::BitStream& bs)
{
	RakString msg;
	bs.Read(msg);
	m_message = msg;
	bs.Read(m_team);
	bs.Read(m_senderId);
	bs.Read(m_source);
}
//---------------------------------------------------------------------------
void ChatMessage::_serialize(BitStream& bs)
{
	bs.Write(RakString(m_message));
	bs.Write(m_team);
	bs.Write(m_senderId);
	bs.Write(m_source);
}
