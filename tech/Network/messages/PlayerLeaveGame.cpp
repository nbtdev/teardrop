/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "PlayerLeaveGame.h"
#include "Stream.h"

using namespace Teardrop;
using namespace Net;

//---------------------------------------------------------------------------
TD_NETMESSAGE_IMPL(PlayerLeaveGame);
//---------------------------------------------------------------------------
PlayerLeaveGame::PlayerLeaveGame()
{
}
//---------------------------------------------------------------------------
PlayerLeaveGame::~PlayerLeaveGame()
{
}
//---------------------------------------------------------------------------
void PlayerLeaveGame::deserialize(Net::Stream& bs)
{
	bs.read(m_playerId);
	bs.read(m_additionalInfo);
}
//---------------------------------------------------------------------------
void PlayerLeaveGame::serialize(Net::Stream& bs)
{
	bs.write(m_playerId);
	bs.write(m_additionalInfo);
}
