/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "PlayerLeaveServer.h"
#include "Stream.h"
#include "Peer.h"
#include "Util/Hash.h"

using namespace Teardrop;
using namespace Net;

//---------------------------------------------------------------------------
TD_NETMESSAGE_IMPL(PlayerLeaveServer);
//---------------------------------------------------------------------------
PlayerLeaveServer::PlayerLeaveServer()
{
	m_id = m_pPeer->getPlayerId();
}
//---------------------------------------------------------------------------
PlayerLeaveServer::~PlayerLeaveServer()
{
}
//---------------------------------------------------------------------------
void PlayerLeaveServer::deserialize(Net::Stream& bs)
{
	bs.read(m_id);
}
//---------------------------------------------------------------------------
void PlayerLeaveServer::serialize(Net::Stream& bs)
{
	bs.write(m_id);
}
