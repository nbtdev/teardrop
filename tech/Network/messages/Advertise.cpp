/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "Advertise.h"
#include "Stream.h"

using namespace Teardrop;
using namespace Net;
//---------------------------------------------------------------------------
TD_NETMESSAGE_IMPL(Advertise);
//---------------------------------------------------------------------------
Advertise::Advertise()
{
}
//---------------------------------------------------------------------------
Advertise::~Advertise()
{
}
//---------------------------------------------------------------------------
void Advertise::deserialize(Net::Stream& bs)
{
	bs.read(m_serverName);
	bs.read(m_mapName);
}
//---------------------------------------------------------------------------
void Advertise::serialize(Net::Stream& bs)
{
	bs.write(m_serverName);
	bs.write(m_mapName);
}
