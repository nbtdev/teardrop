/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "stdafx.h"
#include "PlayerJoinGame.h"

using namespace Teardrop;
using namespace Net;
using namespace RakNet;
//---------------------------------------------------------------------------
TD_NETMESSAGE_IMPL(PlayerJoinGame);
//---------------------------------------------------------------------------
PlayerJoinGame::PlayerJoinGame(const Packet& packet) : Message(packet)
{
}
//---------------------------------------------------------------------------
PlayerJoinGame::~PlayerJoinGame()
{
}
//---------------------------------------------------------------------------
void PlayerJoinGame::_deserialize(RakNet::BitStream& bs)
{
	bs.Read(m_id);
	bs.Read(m_avatarId);

	RakString str;
	bs.Read(str);
	m_avatarClass = str;
	bs.Read(str);
	m_playerName = str;
}
//---------------------------------------------------------------------------
void PlayerJoinGame::_serialize(BitStream& bs)
{
	bs.Write(m_id);
	bs.Write(m_avatarId);

	bs.Write(RakString(m_avatarClass));
	bs.Write(RakString(m_playerName));
}
