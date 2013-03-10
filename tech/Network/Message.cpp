/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "Message.h"
#include "Network.h"
#include "Stream.h"
#include "Peer.h"

//#if defined(_DEBUG)
#include "Util/Environment.h"
#include "Util/Logger.h"
//#endif

using namespace Teardrop;
using namespace Net;
//---------------------------------------------------------------------------
Message::Message()
{
	m_priority = PRIORITY_MEDIUM;
	m_reliability = RELIABILITY_RELIABLE;
	m_class = MESSAGE_USER;
	m_channel = 0;
	m_pPeer = 0;
}
//---------------------------------------------------------------------------
Message::~Message()
{
	delete m_pPeer;
}

bool Message::isProxyMessage()
{
	return false;
}