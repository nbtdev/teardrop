/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "ProxyMessage.h"
#include "Stream.h"
#include "Network.h"

using namespace Teardrop;
using namespace Net;
//---------------------------------------------------------------------------
ProxyMessage::ProxyMessage()
{
	mPlayerId = 0;
}
//---------------------------------------------------------------------------
ProxyMessage::~ProxyMessage()
{
}

bool ProxyMessage::isProxyMessage()
{
	return true;
}