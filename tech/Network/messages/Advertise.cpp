/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "stdafx.h"
#include "Advertise.h"

using namespace Teardrop;
using namespace Net;
using namespace RakNet;
//---------------------------------------------------------------------------
TD_NETMESSAGE_IMPL(Advertise);
//---------------------------------------------------------------------------
Advertise::Advertise(const Packet& packet) : Message(packet)
{
}
//---------------------------------------------------------------------------
Advertise::~Advertise()
{
}
//---------------------------------------------------------------------------
void Advertise::_deserialize(RakNet::BitStream& bs)
{
	RakString name;
	bs.Read(name);
	m_serverName = name;
	bs.Read(name);
	m_mapName = name;
}
//---------------------------------------------------------------------------
void Advertise::_serialize(BitStream& bs)
{
	bs.Write(RakString(m_serverName));
	bs.Write(RakString(m_mapName));
}
