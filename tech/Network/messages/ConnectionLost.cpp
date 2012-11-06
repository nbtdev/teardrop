/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "stdafx.h"
#include "ConnectionLost.h"
#include "Util/Hash.h"

using namespace Teardrop;
using namespace Net;
using namespace RakNet;
//---------------------------------------------------------------------------
ConnectionLost::ConnectionLost(const Packet& packet) 
: Message(packet)
{
	// synthesize a player ID from the guid in the packet
	m_playerId = hashData(g, sizeof(g));
}
//---------------------------------------------------------------------------
ConnectionLost::~ConnectionLost()
{
}
//---------------------------------------------------------------------------
void ConnectionLost::deserialize(RakNet::BitStream& bs)
{
	unsigned char id;
	bs.Read(id); // ID_CONNECTION_LOST
}
//---------------------------------------------------------------------------
void ConnectionLost::serialize(BitStream& bs)
{
	// nothing to serialize -- this is never sent from inside the app
}
//---------------------------------------------------------------------------
void ConnectionLost::_deserialize(RakNet::BitStream& bs)
{
}
//---------------------------------------------------------------------------
void ConnectionLost::_serialize(BitStream& bs)
{
}
