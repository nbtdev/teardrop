/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "PlayerCommand.h"
#include "Stream.h"

using namespace Teardrop;
using namespace Net;

//---------------------------------------------------------------------------
TD_NETMESSAGE_IMPL(PlayerCommand);
//---------------------------------------------------------------------------
PlayerCommand::PlayerCommand()
{
	m_channel = 2;
}
//---------------------------------------------------------------------------
PlayerCommand::~PlayerCommand()
{
}
//---------------------------------------------------------------------------
void PlayerCommand::deserialize(Net::Stream& bs)
{
	bs.read(mPlayerId);

	bs.read(extra[0]);
	bs.read(extra[1]);
	bs.read(extra[2]);
	bs.read(extra[3]);
	bs.read(verb);
}
//---------------------------------------------------------------------------
void PlayerCommand::serialize(Net::Stream& bs)
{
	bs.write(mPlayerId);

	bs.write(extra[0]);
	bs.write(extra[1]);
	bs.write(extra[2]);
	bs.write(extra[3]);
	bs.write(verb);
}
