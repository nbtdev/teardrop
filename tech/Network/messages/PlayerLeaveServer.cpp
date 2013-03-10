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
}
//---------------------------------------------------------------------------
PlayerLeaveServer::~PlayerLeaveServer()
{
}
//---------------------------------------------------------------------------
void PlayerLeaveServer::deserialize(Net::Stream& bs)
{
}
//---------------------------------------------------------------------------
void PlayerLeaveServer::serialize(Net::Stream& bs)
{
}
