/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "stdafx.h"
#include "Unadvertise.h"

using namespace Teardrop;
using namespace Net;
using namespace RakNet;
//---------------------------------------------------------------------------
TD_NETMESSAGE_IMPL(Unadvertise);
//---------------------------------------------------------------------------
Unadvertise::Unadvertise(const Packet& packet) : Message(packet)
{
}
//---------------------------------------------------------------------------
Unadvertise::~Unadvertise()
{
}
//---------------------------------------------------------------------------
void Unadvertise::_deserialize(RakNet::BitStream& bs)
{
}
//---------------------------------------------------------------------------
void Unadvertise::_serialize(BitStream& bs)
{
}
