/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "stdafx.h"
#include "RemotePeer.h"
#include "Util/_String.h"
#include "Util/Environment.h"

using namespace Teardrop;
using namespace Net;
using namespace RakNet;
//---------------------------------------------------------------------------
RemotePeer::RemotePeer()
{
}
//---------------------------------------------------------------------------
RemotePeer::~RemotePeer()
{
}
//---------------------------------------------------------------------------
void RemotePeer::send(Message* pMsg)
{
	Peer::send(pMsg, g);
}
//---------------------------------------------------------------------------
void RemotePeer::disconnect()
{
	Peer::disconnect(this);
}
