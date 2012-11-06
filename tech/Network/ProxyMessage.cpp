/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "stdafx.h"
#include "ProxyMessage.h"
#include "Network.h"

using namespace Teardrop;
using namespace Net;
using namespace RakNet;
//---------------------------------------------------------------------------
ProxyMessage::ProxyMessage(const Packet& packet)
: Message(packet)
{
	m_playerId = 0;
}
//---------------------------------------------------------------------------
ProxyMessage::ProxyMessage()
{
	m_playerId = 0;
}
//---------------------------------------------------------------------------
ProxyMessage::~ProxyMessage()
{
}
//---------------------------------------------------------------------------
void ProxyMessage::deserialize(BitStream& bs)
{
	Message::deserialize(bs);
	bs.Read(m_playerId);
}
//---------------------------------------------------------------------------
void ProxyMessage::serialize(BitStream& bs)
{
	Message::serialize(bs);
	bs.Write(m_playerId);
}
