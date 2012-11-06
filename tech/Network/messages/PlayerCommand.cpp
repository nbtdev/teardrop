/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "stdafx.h"
#include "PlayerCommand.h"

using namespace Teardrop;
using namespace Net;
using namespace RakNet;
//---------------------------------------------------------------------------
TD_PROXYMESSAGE_IMPL(PlayerCommand);
//---------------------------------------------------------------------------
PlayerCommand::PlayerCommand(const Packet& packet) 
: ProxyMessage(packet)
{
	m_channel = 2;
}
//---------------------------------------------------------------------------
PlayerCommand::~PlayerCommand()
{
}
//---------------------------------------------------------------------------
void PlayerCommand::_deserialize(RakNet::BitStream& bs)
{
	bs.Read(extra[0]);
	bs.Read(extra[1]);
	bs.Read(extra[2]);
	bs.Read(extra[3]);
	bs.Read(verb);
}
//---------------------------------------------------------------------------
void PlayerCommand::_serialize(BitStream& bs)
{
	bs.Write(extra[0]);
	bs.Write(extra[1]);
	bs.Write(extra[2]);
	bs.Write(extra[3]);
	bs.Write(verb);
}
