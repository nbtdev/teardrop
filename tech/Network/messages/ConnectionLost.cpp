/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "ConnectionLost.h"
#include "Peer.h"
#include "Stream.h"
#include "Util/Hash.h"

using namespace Teardrop;
using namespace Net;
//---------------------------------------------------------------------------
ConnectionLost::ConnectionLost() 
{
	// synthesize a player ID from the guid in the packet
	mPlayerId = m_pPeer->getPlayerId();
}
//---------------------------------------------------------------------------
ConnectionLost::~ConnectionLost()
{
}
//---------------------------------------------------------------------------
void ConnectionLost::deserialize(Net::Stream& bs)
{
	unsigned char id;
	bs.read(id); // "connection lost" message ID
}
//---------------------------------------------------------------------------
void ConnectionLost::serialize(Net::Stream& bs)
{
	// nothing to serialize -- this is never sent from inside the app
}

