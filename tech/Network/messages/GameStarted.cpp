/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "stdafx.h"
#include "GameStarted.h"

using namespace Teardrop;
using namespace Net;
using namespace RakNet;
//---------------------------------------------------------------------------
TD_NETMESSAGE_IMPL(GameStarted);
//---------------------------------------------------------------------------
GameStarted::GameStarted(const Packet& packet) : Message(packet)
{
	m_worldId = 0;
}
//---------------------------------------------------------------------------
GameStarted::~GameStarted()
{
}
//---------------------------------------------------------------------------
void GameStarted::_deserialize(RakNet::BitStream& bs)
{
	bs.Read(m_worldId);
}
//---------------------------------------------------------------------------
void GameStarted::_serialize(BitStream& bs)
{
	bs.Write(m_worldId);
}
