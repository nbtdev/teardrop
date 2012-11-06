/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "stdafx.h"
#include "PingResponse.h"
#include "GetTime.h"

using namespace Teardrop;
using namespace Net;
using namespace RakNet;
//---------------------------------------------------------------------------
PingResponse::PingResponse(const Packet& packet) : Message(packet)
{
	// need to zero out the m_data member, because ping responses from a 
	// server before the server has set up its discovery data, will return
	// a zero-length data block...which will leave this memory uninitialized
	// in the deserialize below...
	memset(&m_data, 0, sizeof(m_data));
}
//---------------------------------------------------------------------------
PingResponse::~PingResponse()
{
}
//---------------------------------------------------------------------------
void PingResponse::deserialize(RakNet::BitStream& bs)
{
	unsigned char id;
	bs.Read(id); // ID_PONG

	RakNetTime time;
	bs.Read(time); // system time from remote response

	// read the offline ping response data, if any
	bs.Read((char*)&m_data, sizeof(m_data));

	m_time = RakNet::GetTime() - time;
}
//---------------------------------------------------------------------------
void PingResponse::serialize(BitStream& bs)
{
	// nothing to serialize -- this is never sent from inside the app
}
//---------------------------------------------------------------------------
void PingResponse::_deserialize(RakNet::BitStream& bs)
{
}
//---------------------------------------------------------------------------
void PingResponse::_serialize(BitStream& bs)
{
}
