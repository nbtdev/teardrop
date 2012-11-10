/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "UpdateServerState.h"
#include "Stream.h"
#include "Util/Environment.h"

using namespace Teardrop;
using namespace Net;

//---------------------------------------------------------------------------
TD_NETMESSAGE_IMPL(UpdateServerState);
//---------------------------------------------------------------------------
UpdateServerState::UpdateServerState()
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
void UpdateServerState::deserialize(Net::Stream& bs)
{
	unsigned char numPlayers;
	unsigned short ping;

	bs.read(m_updateOrRemove); // 0 for update/add, 1 for remove
	bs.read(m_ip);
	bs.read(m_mapName);
	bs.read(m_serverName);
	bs.read(m_mapId);
	bs.read(ping);
	m_ping = ping;
	bs.read(numPlayers);
	m_numPlayers = numPlayers;
	bs.read(m_maxNumTeams);
	bs.read(m_bInProgress);
}
//---------------------------------------------------------------------------
void UpdateServerState::serialize(Net::Stream& bs)
{
	char update = 0; // removes happen a different way
	bs.write(update);
	bs.write(m_ip);
	bs.write(m_mapName);
	bs.write(m_serverName);
	bs.write(m_mapId);
	bs.write(unsigned short(m_ping));
	bs.write(unsigned char(m_numPlayers));
	bs.write(m_maxNumTeams);
	bs.write(m_bInProgress);
}
