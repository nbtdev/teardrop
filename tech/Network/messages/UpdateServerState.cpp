/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#include "stdafx.h"
#include "UpdateServerState.h"
#include "Util/include/Environment.h"

using namespace CoS;
using namespace Net;
using namespace RakNet;
//---------------------------------------------------------------------------
COS_NETMESSAGE_IMPL(UpdateServerState);
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
