/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "stdafx.h"
#include "ConnectionRequestAccepted.h"

using namespace Teardrop;
using namespace Net;
using namespace RakNet;
//---------------------------------------------------------------------------
ConnectionRequestAccepted::ConnectionRequestAccepted(const Packet& packet) 
: Message(packet)
{
}
//---------------------------------------------------------------------------
ConnectionRequestAccepted::~ConnectionRequestAccepted()
{
}
//---------------------------------------------------------------------------
void ConnectionRequestAccepted::deserialize(RakNet::BitStream& bs)
{
	unsigned char id;
	bs.Read(id); // ID_CONNECTION_REQUEST_ACCEPTED
}
//---------------------------------------------------------------------------
void ConnectionRequestAccepted::serialize(BitStream& bs)
{
	// nothing to serialize -- this is never sent from inside the app
}
//---------------------------------------------------------------------------
void ConnectionRequestAccepted::_deserialize(RakNet::BitStream& bs)
{
}
//---------------------------------------------------------------------------
void ConnectionRequestAccepted::_serialize(BitStream& bs)
{
}
