/******************************************************************************
Copyright (c) 2015 Teardrop Games

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
	bs.read(mPlayerId);
	bs.read(m_playerName);
	bs.read(m_playerCharClass);
	bs.read(m_playerChar);
	bs.read(m_playerEntityVariant);
}
//---------------------------------------------------------------------------
void PlayerJoinServer::serialize(Net::Stream& bs)
{
	bs.write(mPlayerId);
	bs.write(m_playerName);
	bs.write(m_playerCharClass);
	bs.write(m_playerChar);
	bs.write(m_playerEntityVariant);
}
