/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "stdafx.h"
#include "PlayerLeaveServer.h"
#include "Util/Hash.h"

using namespace Teardrop;
using namespace Net;
using namespace RakNet;
//---------------------------------------------------------------------------
TD_NETMESSAGE_IMPL(PlayerLeaveServer);
//---------------------------------------------------------------------------
PlayerLeaveServer::PlayerLeaveServer(const Packet& packet) : Message(packet)
{
	m_id = hashData(g, sizeof(g));
}
//---------------------------------------------------------------------------
PlayerLeaveServer::~PlayerLeaveServer()
{
}
//---------------------------------------------------------------------------
void PlayerLeaveServer::_deserialize(RakNet::BitStream& bs)
{
	bs.Read(m_id);
}
//---------------------------------------------------------------------------
void PlayerLeaveServer::_serialize(BitStream& bs)
{
	bs.Write(m_id);
}
