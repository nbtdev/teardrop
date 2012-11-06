/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "stdafx.h"
#include "PlayerJoinServer.h"
#include "Util/Hash.h"

using namespace Teardrop;
using namespace Net;
using namespace RakNet;
//---------------------------------------------------------------------------
TD_NETMESSAGE_IMPL(PlayerJoinServer);
//---------------------------------------------------------------------------
PlayerJoinServer::PlayerJoinServer(const Packet& packet) : Message(packet)
{
	m_id = 0;
	m_playerChar = 0;
}
//---------------------------------------------------------------------------
PlayerJoinServer::~PlayerJoinServer()
{
}
//---------------------------------------------------------------------------
void PlayerJoinServer::_deserialize(RakNet::BitStream& bs)
{
	bs.Read(m_id);

	RakString str;
	bs.Read(str);
	m_playerName = str;
	bs.Read(str);
	m_playerCharClass = str;
	bs.Read(m_playerChar);
	bs.Read(m_playerEntityVariant);
}
//---------------------------------------------------------------------------
void PlayerJoinServer::_serialize(BitStream& bs)
{
	bs.Write(m_id);
	bs.Write(RakString(m_playerName));
	bs.Write(RakString(m_playerCharClass));
	bs.Write(m_playerChar);
	bs.Write(m_playerEntityVariant);
}
