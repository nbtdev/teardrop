/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "PlayerJoinServer.h"
#include "Stream.h"
#include "Util/Hash.h"

using namespace Teardrop;
using namespace Net;

//---------------------------------------------------------------------------
TD_NETMESSAGE_IMPL(PlayerJoinServer);
//---------------------------------------------------------------------------
PlayerJoinServer::PlayerJoinServer()
{
	m_playerChar = 0;
	m_playerEntityVariant = 0;
}
//---------------------------------------------------------------------------
PlayerJoinServer::~PlayerJoinServer()
{
}
//---------------------------------------------------------------------------
void PlayerJoinServer::deserialize(Net::Stream& bs)
{
	bs.read(m_playerName);
	bs.read(m_playerCharClass);
	bs.read(m_playerChar);
	bs.read(m_playerEntityVariant);
}
//---------------------------------------------------------------------------
void PlayerJoinServer::serialize(Net::Stream& bs)
{
	bs.write(m_playerName);
	bs.write(m_playerCharClass);
	bs.write(m_playerChar);
	bs.write(m_playerEntityVariant);
}
