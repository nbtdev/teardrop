/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "stdafx.h"
#include "UpdateServerState.h"
#include "Util/Environment.h"

using namespace Teardrop;
using namespace Net;
using namespace RakNet;
//---------------------------------------------------------------------------
TD_NETMESSAGE_IMPL(UpdateServerState);
//---------------------------------------------------------------------------
UpdateServerState::UpdateServerState(const Packet& packet) : Message(packet)
{
	m_bInProgress = false;
	m_updateOrRemove = 0; // default to update
	m_numPlayers = 0;
	m_ip = 0;
	m_mapId = 0;
	m_maxNumTeams = -1;
}
//---------------------------------------------------------------------------
UpdateServerState::~UpdateServerState()
{
}
//---------------------------------------------------------------------------
void UpdateServerState::_deserialize(RakNet::BitStream& bs)
{
	RakString name;
	unsigned char numPlayers;
	unsigned short ping;

	bs.Read(m_updateOrRemove); // 0 for update/add, 1 for remove
	bs.Read(m_ip);
	bs.Read(name);
	m_mapName = name;
	bs.Read(name);
	m_serverName = name;
	bs.Read(m_mapId);
	bs.Read(ping);
	m_ping = ping;
	bs.Read(numPlayers);
	m_numPlayers = numPlayers;
	bs.Read(m_maxNumTeams);
	bs.Read(m_bInProgress);
}
//---------------------------------------------------------------------------
void UpdateServerState::_serialize(BitStream& bs)
{
	char update = 0; // removes happen a different way
	bs.Write(update);
	bs.Write(m_ip);
	bs.Write(RakString(m_mapName));
	bs.Write(RakString(m_serverName));
	bs.Write(m_mapId);
	bs.Write(unsigned short(m_ping));
	bs.Write(unsigned char(m_numPlayers));
	bs.Write(m_maxNumTeams);
	bs.Write(m_bInProgress);
}
