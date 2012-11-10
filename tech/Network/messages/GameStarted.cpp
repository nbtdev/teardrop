/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "GameStarted.h"
#include "Stream.h"

using namespace Teardrop;
using namespace Net;

//---------------------------------------------------------------------------
TD_NETMESSAGE_IMPL(GameStarted);
//---------------------------------------------------------------------------
GameStarted::GameStarted()
{
	m_worldId = 0;
}
//---------------------------------------------------------------------------
GameStarted::~GameStarted()
{
}
//---------------------------------------------------------------------------
void GameStarted::deserialize(Net::Stream& bs)
{
	bs.read(m_worldId);
}
//---------------------------------------------------------------------------
void GameStarted::serialize(Net::Stream& bs)
{
	bs.write(m_worldId);
}
