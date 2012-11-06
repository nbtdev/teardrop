/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "stdafx.h"
#include "PlayerPositionSync.h"

using namespace Teardrop;
using namespace Net;
using namespace RakNet;
//---------------------------------------------------------------------------
TD_PROXYMESSAGE_IMPL(PlayerPositionSync);
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
