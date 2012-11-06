/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "stdafx.h"
#include "GameEnded.h"

using namespace Teardrop;
using namespace Net;
using namespace RakNet;
//---------------------------------------------------------------------------
TD_NETMESSAGE_IMPL(GameEnded);
//---------------------------------------------------------------------------
GameEnded::GameEnded(const Packet& packet) : Message(packet)
{
	m_channel = 2;
}
//---------------------------------------------------------------------------
GameEnded::~GameEnded()
{
}
//---------------------------------------------------------------------------
void GameEnded::_deserialize(RakNet::BitStream& bs)
{
}
//---------------------------------------------------------------------------
void GameEnded::_serialize(BitStream& bs)
{
}
