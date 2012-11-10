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
	m_playerId = 0;
}
//---------------------------------------------------------------------------
ProxyMessage::~ProxyMessage()
{
}
//---------------------------------------------------------------------------
void ProxyMessage::deserialize(Net::Stream& bs)
{
	Message::deserialize(bs);
	bs.read(m_playerId);
}
//---------------------------------------------------------------------------
void ProxyMessage::serialize(Net::Stream& bs)
{
	Message::serialize(bs);
	bs.write(m_playerId);
}
