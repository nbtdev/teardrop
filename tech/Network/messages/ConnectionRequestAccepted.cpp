/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "ConnectionRequestAccepted.h"
#include "Stream.h"

using namespace Teardrop;
using namespace Net;

//---------------------------------------------------------------------------
ConnectionRequestAccepted::ConnectionRequestAccepted()
{
}
//---------------------------------------------------------------------------
ConnectionRequestAccepted::~ConnectionRequestAccepted()
{
}
//---------------------------------------------------------------------------
void ConnectionRequestAccepted::deserialize(Net::Stream& bs)
{
	unsigned char id;
	bs.read(id); // ID_CONNECTION_REQUEST_ACCEPTED
}
//---------------------------------------------------------------------------
void ConnectionRequestAccepted::serialize(Net::Stream& bs)
{
	// nothing to serialize -- this is never sent from inside the app
}
