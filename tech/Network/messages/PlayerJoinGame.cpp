/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "PlayerJoinGame.h"
#include "Stream.h"

using namespace Teardrop;
using namespace Net;

//---------------------------------------------------------------------------
TD_NETMESSAGE_IMPL(PlayerJoinGame);
//---------------------------------------------------------------------------
PlayerJoinGame::PlayerJoinGame()
{
}
//---------------------------------------------------------------------------
PlayerJoinGame::~PlayerJoinGame()
{
}
//---------------------------------------------------------------------------
void PlayerJoinGame::deserialize(Net::Stream& bs)
{
	bs.read(m_id);
	bs.read(m_avatarId);
	bs.read(m_avatarClass);
	bs.read(m_playerName);
}
//---------------------------------------------------------------------------
void PlayerJoinGame::serialize(Net::Stream& bs)
{
	bs.write(m_id);
	bs.write(m_avatarId);
	bs.write(m_avatarClass);
	bs.write(m_playerName);
}
