/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "InterrogateServer.h"
#include "Stream.h"

using namespace Teardrop;
using namespace Net;

//---------------------------------------------------------------------------
TD_NETMESSAGE_IMPL(InterrogateServer);
//---------------------------------------------------------------------------
InterrogateServer::InterrogateServer()
{
}
//---------------------------------------------------------------------------
InterrogateServer::~InterrogateServer()
{
}
//---------------------------------------------------------------------------
void InterrogateServer::deserialize(Net::Stream& bs)
{
	bs.read(m_addr);
}
//---------------------------------------------------------------------------
void InterrogateServer::serialize(Net::Stream& bs)
{
	bs.write(m_addr);
}
