/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "stdafx.h"
#include "PlayerLeaveGame.h"

using namespace Teardrop;
using namespace Net;
using namespace RakNet;
//---------------------------------------------------------------------------
TD_NETMESSAGE_IMPL(PlayerLeaveGame);
//---------------------------------------------------------------------------
PlayerLeaveGame::PlayerLeaveGame(const Packet& packet) : Message(packet)
{
}
//---------------------------------------------------------------------------
PlayerLeaveGame::~PlayerLeaveGame()
{
}
//---------------------------------------------------------------------------
void PlayerLeaveGame::_deserialize(RakNet::BitStream& bs)
{
	bs.Read(m_playerId);

	RakString str;
	bs.Read(str);

	m_additionalInfo = str;
}
//---------------------------------------------------------------------------
void PlayerLeaveGame::_serialize(BitStream& bs)
{
	bs.Write(m_playerId);
	bs.Write(RakString(m_additionalInfo));
}
