/******************************************************************************
Copyright (c) 2018 Teardrop Games

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
******************************************************************************/
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
