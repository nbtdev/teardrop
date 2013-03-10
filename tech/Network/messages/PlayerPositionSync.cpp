/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "PlayerPositionSync.h"
#include "Stream.h"

using namespace Teardrop;
using namespace Net;

//---------------------------------------------------------------------------
TD_NETMESSAGE_IMPL(PlayerPositionSync);
//---------------------------------------------------------------------------
PlayerPositionSync::PlayerPositionSync()
{
	m_channel = 2;
}
//---------------------------------------------------------------------------
PlayerPositionSync::~PlayerPositionSync()
{
}
//---------------------------------------------------------------------------
void PlayerPositionSync::deserialize(Net::Stream& bs)
{
	bs.read(mPlayerId);

	// for now, for dev, we'll just transfer the whole XYZ and quat
	bs.read(m_position.x);
	bs.read(m_position.y);
	bs.read(m_position.z);
	m_position.w = 0;

	bs.read(m_orientation.w);
	bs.read(m_orientation.x);
	bs.read(m_orientation.y);
	bs.read(m_orientation.z);

	bs.read(m_twistAngle);
}
//---------------------------------------------------------------------------
void PlayerPositionSync::serialize(Net::Stream& bs)
{
	bs.write(mPlayerId);

	// for now, for dev, we'll just transfer the whole XYZ and quat
	bs.write(m_position.x);
	bs.write(m_position.y);
	bs.write(m_position.z);

	bs.write(m_orientation.w);
	bs.write(m_orientation.x);
	bs.write(m_orientation.y);
	bs.write(m_orientation.z);

	bs.write(m_twistAngle);
}
