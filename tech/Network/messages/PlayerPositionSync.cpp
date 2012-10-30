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
#include "PlayerPositionSync.h"

using namespace CoS;
using namespace Net;
using namespace RakNet;
//---------------------------------------------------------------------------
COS_PROXYMESSAGE_IMPL(PlayerPositionSync);
//---------------------------------------------------------------------------
PlayerPositionSync::PlayerPositionSync(const Packet& packet) 
: ProxyMessage(packet)
{
	m_channel = 2;
}
//---------------------------------------------------------------------------
PlayerPositionSync::~PlayerPositionSync()
{
}
//---------------------------------------------------------------------------
void PlayerPositionSync::_deserialize(RakNet::BitStream& bs)
{
	// for now, for dev, we'll just transfer the whole XYZ and quat
	bs.Read(m_position.x);
	bs.Read(m_position.y);
	bs.Read(m_position.z);
	m_position.w = 0;

	bs.Read(m_orientation.w);
	bs.Read(m_orientation.x);
	bs.Read(m_orientation.y);
	bs.Read(m_orientation.z);

	bs.Read(m_twistAngle);
}
//---------------------------------------------------------------------------
void PlayerPositionSync::_serialize(BitStream& bs)
{
	// for now, for dev, we'll just transfer the whole XYZ and quat
	bs.Write(m_position.x);
	bs.Write(m_position.y);
	bs.Write(m_position.z);

	bs.Write(m_orientation.w);
	bs.Write(m_orientation.x);
	bs.Write(m_orientation.y);
	bs.Write(m_orientation.z);

	bs.Write(m_twistAngle);
}
