/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "stdafx.h"
#include "InterrogateServer.h"

using namespace Teardrop;
using namespace Net;
using namespace RakNet;
//---------------------------------------------------------------------------
TD_NETMESSAGE_IMPL(InterrogateServer);
//---------------------------------------------------------------------------
InterrogateServer::InterrogateServer(const Packet& packet) : Message(packet)
{
}
//---------------------------------------------------------------------------
InterrogateServer::~InterrogateServer()
{
}
//---------------------------------------------------------------------------
void InterrogateServer::_deserialize(RakNet::BitStream& bs)
{
	bs.Read(m_addr);
}
//---------------------------------------------------------------------------
void InterrogateServer::_serialize(BitStream& bs)
{
	bs.Write(m_addr);
}
