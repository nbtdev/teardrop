/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "stdafx.h"
#include "UpdatePlayerState.h"

using namespace Teardrop;
using namespace Net;
using namespace RakNet;
//---------------------------------------------------------------------------
TD_NETMESSAGE_IMPL(UpdatePlayerState);
//---------------------------------------------------------------------------
UpdatePlayerState::UpdatePlayerState(const Packet& packet) : Message(packet)
{
}
//---------------------------------------------------------------------------
UpdatePlayerState::~UpdatePlayerState()
{
}
//---------------------------------------------------------------------------
void UpdatePlayerState::_deserialize(RakNet::BitStream& bs)
{
	bs.Read(m_id);
	bs.Read(m_playerTeam);
	RakString str;
	bs.Read(str);
	m_playerName = str;
	bs.Read(str);
	m_playerCharClass = str;
	bs.Read(m_playerEntityVariant);
	bs.Read(m_playerChar);
	bs.Read(m_bReady);
}
//---------------------------------------------------------------------------
void UpdatePlayerState::_serialize(BitStream& bs)
{
	bs.Write(m_id);
	bs.Write(m_playerTeam);
	bs.Write(RakString(m_playerName));
	bs.Write(RakString(m_playerCharClass));
	bs.Write(m_playerEntityVariant);
	bs.Write(m_playerChar);
	bs.Write(m_bReady);
}
