/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "ChatMessage.h"
#include "Stream.h"

using namespace Teardrop;
using namespace Net;
//---------------------------------------------------------------------------
TD_NETMESSAGE_IMPL(ChatMessage);
//---------------------------------------------------------------------------
ChatMessage::ChatMessage()
{
	m_team = 0; // default to "broadcast"
}
//---------------------------------------------------------------------------
ChatMessage::~ChatMessage()
{
}
//---------------------------------------------------------------------------
void ChatMessage::deserialize(Net::Stream& bs)
{
	bs.read(m_message);
	bs.read(m_team);
	bs.read(m_senderId);
	bs.read(m_source);
}
//---------------------------------------------------------------------------
void ChatMessage::serialize(Net::Stream& bs)
{
	bs.write(m_message);
	bs.write(m_team);
	bs.write(m_senderId);
	bs.write(m_source);
}
